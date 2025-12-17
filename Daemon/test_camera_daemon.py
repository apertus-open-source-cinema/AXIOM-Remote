import pytest
from camera_daemon import handle_command, params, Param

# -----------------------------
# HELPER
# -----------------------------
def expect_ok(cmd, expected=None):
    response = handle_command(cmd)
    assert response.startswith("OK")
    if expected is not None:
        assert expected in response

def expect_err(cmd, code):
    response = handle_command(cmd)
    assert response.startswith(f"ERR {code}")

# -----------------------------
# 1. HELLO
# -----------------------------
def test_hello():
    expect_ok("HELLO", "proto=1.0")

# -----------------------------
# 2. LIST
# -----------------------------
def test_list():
    resp = handle_command("LIST")
    assert resp.startswith("OK")
    for name in params.keys():
        assert name in resp

# -----------------------------
# 3. GET valid
# -----------------------------
def test_get_valid():
    for name, p in params.items():
        resp = handle_command(f"GET {name}")
        assert resp.startswith("OK")
        assert str(p.value) in resp

# -----------------------------
# 4. GET unknown_param
# -----------------------------
def test_get_unknown():
    expect_err("GET fake_param", "unknown_param")

# -----------------------------
# 5. SET valid
# -----------------------------
def test_set_valid():
    # Only writable params
    writable = [p for p in params.values() if p.rw]
    for p in writable:
        # Skip focus_pos if focus_mode != manual
        if p.name == "focus_pos" and params["focus_mode"].value != "manual":
            continue

        if p.type == "int":
            resp = handle_command(f"SET {p.name} {p.min}")
            assert resp == "OK"
        elif p.type == "enum":
            resp = handle_command(f"SET {p.name} {p.values[0]}")
            assert resp == "OK"

# -----------------------------
# 6. SET read_only
# -----------------------------
def test_set_read_only():
    for p in params.values():
        if not p.rw:
            expect_err(f"SET {p.name} 123", "read_only")

# -----------------------------
# 7. SET out_of_range / invalid_value
# -----------------------------
def test_set_out_of_range():
    for p in params.values():
        if p.rw and p.type in ["int", "float"]:
            expect_err(f"SET {p.name} {p.max + 1}", "out_of_range")
            expect_err(f"SET {p.name} {p.min - 1}", "out_of_range")

def test_set_invalid_enum():
    for p in params.values():
        if p.type == "enum":
            expect_err(f"SET {p.name} INVALID", "invalid_enum")

# -----------------------------
# 8. Invalid syntax
# -----------------------------
def test_invalid_syntax():
    expect_err("", "empty")
    expect_err("FOO", "syntax")
    expect_err("GET", "syntax")
    expect_err("SET exposure_us", "syntax")
    expect_err("INFO", "syntax")

# -----------------------------
# 9. Invalid state (focus_pos when focus_mode != manual)
# -----------------------------
def test_invalid_state():
    # Force focus_mode to auto
    params["focus_mode"].value = "auto"
    expect_err("SET focus_pos 50", "invalid_state")
    # Switch back to manual
    params["focus_mode"].value = "manual"
    expect_ok("SET focus_pos 50")

# -----------------------------
# 10. INFO
# -----------------------------
def test_info():
    for name, p in params.items():
        resp = handle_command(f"INFO {name}")
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
def test_too_long():
    long_line = "A" * 200
    expect_err(long_line, "too_long")
