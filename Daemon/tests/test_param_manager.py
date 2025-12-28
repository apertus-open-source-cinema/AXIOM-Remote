import logging
from pathlib import Path

import pytest

from param_manager import ParamManager
from fs_backend import CameraBackend


def test_from_parameter_db_parses_values_and_ranges(tmp_path, caplog):
    # Prepare a fake parameter DB entry
    parameter_db = {
        str(tmp_path / "p1"): {
            "type": "int",
            "writable": True,
            "range": "0 10 1",
            "map": ["a", "b", "c"],
        },
        str(tmp_path / "p2"): {
            # missing range and map
            "type": "string",
            "writable": False,
        },
        str(tmp_path / "p3"): {
            # malformed range
            "type": "float",
            "range": "not_a_number",
        },
    }

    backend = CameraBackend()

    with caplog.at_level(logging.WARNING):
        manager = ParamManager.from_parameter_db(parameter_db, backend)

    # p1 assertions
    p1 = manager.get_param("p1")
    assert p1.name == "p1"
    assert p1.type == "int"
    assert p1.path == Path(str(tmp_path / "p1"))
    assert p1.rw is True
    assert p1.min == 0.0
    assert p1.max == 10.0
    assert p1.step == 1.0
    assert p1.values == ["a", "b", "c"]

    # p2 assertions
    p2 = manager.get_param("p2")
    assert p2.name == "p2"
    assert p2.type == "string"
    assert p2.rw is False
    assert p2.min is None
    assert p2.max is None
    assert p2.values is None

    # p3 should be present but range parsing should have logged a warning
    p3 = manager.get_param("p3")
    assert p3.name == "p3"
    assert any("Failed to parse range for p3" in rec.message for rec in caplog.records)


def test_from_parameter_db_handles_empty_db():
    backend = CameraBackend()
    manager = ParamManager.from_parameter_db({}, backend)
    with pytest.raises(Exception):
        # asking for a non-existent param should raise
        manager.get_param("does_not_exist")
