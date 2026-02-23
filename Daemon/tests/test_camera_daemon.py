import pytest
import pyfakefs.pytest_plugin  # Import pyfakefs
from camera_daemon import CameraDaemon
from param_manager import Param, ParamManager
from fs_backend import CameraBackend  # Import CameraBackend
from unittest.mock import MagicMock
from pathlib import Path


# -----------------------------
# FIXTURES
# -----------------------------
@pytest.fixture
def params(fs):
    # Create the base directory for parameters
    base_param_path = Path("/tmp/params")
    fs.create_dir(base_param_path)

    p1 = Param(
        name="exposure_us",
        type="int",
        path=base_param_path / "exposure_us",
        min=100,
        max=10000,
        rw=True,
    )
    p2 = Param(
        name="focus_mode",
        type="enum",
        path=base_param_path / "focus_mode",
        values=["auto", "manual"],
        rw=True,
    )
    p3 = Param(
        name="focus_pos",
        type="int",
        path=base_param_path / "focus_pos",
        min=0,
        max=100,
        rw=True,
    )
    p4 = Param(
        name="read_only_param",
        type="int",
        path=base_param_path / "read_only_param",
        rw=False,
        min=0,
        max=10,
    )

    # Mock initial values and create fake files
    p1.value = 5000
    p2.value = "manual"
    p3.value = 50
    p4.value = 5

    param_list = [p1, p2, p3, p4]

    for p in param_list:
        fs.create_dir(p.path)  # Create the parameter directory
        fs.create_file(p.path / "value", contents=str(p.value))  # Create value file

        # Create type file
        fs.create_file(p.path / "ty", contents=p.type)

        # Create writable file
        fs.create_file(p.path / "writable", contents=str(p.rw).lower())

        # Create range file if applicable
        if p.min is not None and p.max is not None:
            fs.create_file(p.path / "range", contents=f"{p.min}-{p.max}")
        elif p.min is not None:
            fs.create_file(p.path / "range", contents=f"{p.min}-")
        elif p.max is not None:
            fs.create_file(p.path / "range", contents=f"-{p.max}")

        # Create map file for enums
        if p.values is not None:
            fs.create_file(p.path / "map", contents="\n".join(p.values))

    return {"exposure_us": p1, "focus_mode": p2, "focus_pos": p3, "read_only_param": p4}


@pytest.fixture
def mock_backend():
    return CameraBackend()


@pytest.fixture
def daemon(params, mock_backend):
    manager = ParamManager(params, mock_backend)
    return CameraDaemon(manager)


# -----------------------------
# HELPER
# -----------------------------
def expect_ok(daemon, cmd, expected=None):
    response = daemon.handle_command(cmd)
    assert response.startswith("OK")
    if expected is not None:
        assert expected in response


def expect_err(daemon, cmd, code):
    response = daemon.handle_command(cmd)
    assert response.startswith(f"ERR {code}")


# -----------------------------
# 1. HELLO
# -----------------------------
def test_hello(daemon):
    expect_ok(daemon, "HELLO", "proto=1.0")


# -----------------------------
# 2. LIST
# -----------------------------
def test_list(daemon, params):
    resp = daemon.handle_command("LIST")
    assert resp.startswith("OK")
    for name in params.keys():
        assert name in resp


# -----------------------------
# 3. GET valid
# -----------------------------
def test_get_valid(daemon, params):
    for name, p in params.items():
        resp = daemon.handle_command(f"GET {name}")
        assert resp.startswith("OK")
        assert str(p.value) in resp


# -----------------------------
# 4. GET unknown_param
# -----------------------------
def test_get_unknown(daemon):
    expect_err(daemon, "GET fake_param", "unknown_param")


# -----------------------------
# 5. SET valid
# -----------------------------
def test_set_valid(daemon, params):
    # Only writable params
    writable = [p for p in params.values() if p.rw]
    for p in writable:
        # Skip focus_pos if focus_mode != manual
        if p.name == "focus_pos" and params["focus_mode"].value != "manual":
            continue

        if p.type == "int":
            val = p.min
            resp = daemon.handle_command(f"SET {p.name} {val}")
            assert resp == "OK"
            p.value = val
        elif p.type == "enum":
            val = p.values[0]
            resp = daemon.handle_command(f"SET {p.name} {val}")
            assert resp == "OK"
            p.value = val


# -----------------------------
# 6. SET read_only
# -----------------------------
def test_set_read_only(daemon, params):
    for p in params.values():
        if not p.rw:
            expect_err(daemon, f"SET {p.name} 123", "read_only")


# -----------------------------
# 7. SET out_of_range / invalid_value
# -----------------------------
def test_set_out_of_range(daemon, params):
    for p in params.values():
        if p.rw and p.type in ["int", "float"]:
            expect_err(daemon, f"SET {p.name} {p.max + 1}", "out_of_range")
            expect_err(daemon, f"SET {p.name} {p.min - 1}", "out_of_range")


def test_set_invalid_enum(daemon, params):
    for p in params.values():
        if p.type == "enum":
            expect_err(daemon, f"SET {p.name} INVALID", "invalid_enum")


# -----------------------------
# 8. Invalid syntax
# -----------------------------
def test_invalid_syntax(daemon):
    expect_err(daemon, "", "empty")
    expect_err(daemon, "FOO", "syntax")
    expect_err(daemon, "GET", "syntax")
    expect_err(daemon, "SET exposure_us", "syntax")
    expect_err(daemon, "INFO", "syntax")


# -----------------------------
# 9. Invalid state (focus_pos when focus_mode != manual)
# -----------------------------
def test_invalid_state(daemon, params):
    # Force focus_mode to auto
    expect_ok(daemon, "SET focus_mode auto")

    expect_err(daemon, "SET focus_pos 50", "invalid_state")

    # Switch back to manual
    expect_ok(daemon, "SET focus_mode manual")
    expect_ok(daemon, "SET focus_pos 50")


# -----------------------------
# 10. INFO
# -----------------------------
def test_info(daemon, params):
    for name, p in params.items():
        resp = daemon.handle_command(f"INFO {name}")
        assert resp.startswith("OK")
        assert f"type={p.type}" in resp
        if p.min is not None:
            assert f"min={p.min}" in resp
        if p.max is not None:
            assert f"max={p.max}" in resp
        if p.values is not None:
            for v in p.values:
                assert v in resp


# -----------------------------
# 11. Line too long
# -----------------------------
def test_too_long(daemon):
    long_line = "A" * 200
    expect_err(daemon, long_line, "too_long")
