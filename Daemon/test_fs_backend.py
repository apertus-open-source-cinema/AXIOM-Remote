import pytest
import os
import shutil
from fs_backend import FSBackend

@pytest.fixture
def fs_backend():
    return FSBackend()

@pytest.fixture
def temp_fs(tmp_path):
    """Creates a temporary filesystem structure for testing."""
    base = tmp_path / "nctrl_api"
    base.mkdir()
    
    # Create param1
    p1 = base / "param1"
    p1.mkdir()
    (p1 / "value").write_text("100")
    (p1 / "ty").write_text("int")
    (p1 / "writable").write_text("true")
    
    # Create nested param2
    nested = base / "subdir"
    nested.mkdir()
    p2 = nested / "param2"
    p2.mkdir()
    (p2 / "value").write_text("auto")
    (p2 / "ty").write_text("enum")
    (p2 / "map").write_text("auto\nmanual")
    
    return base

def test_scan_parameters_valid(fs_backend, temp_fs):
    params_to_find = ["param1", "param2"]
    result, errors = fs_backend.scan_parameters(params_to_find, str(temp_fs))
    
    assert len(result) == 2
    assert not errors
    
    # Check param1
    p1_path = str(temp_fs / "param1")
    assert p1_path in result
    assert result[p1_path]['value'] == "100"
    assert result[p1_path]['type'] == "int"
    assert result[p1_path]['writable'] is True
    
    # Check param2
    p2_path = str(temp_fs / "subdir" / "param2")
    assert p2_path in result
    assert result[p2_path]['value'] == "auto"
    assert result[p2_path]['type'] == "enum"
    assert result[p2_path]['map'] == ["auto", "manual"]

def test_scan_parameters_missing_base_path(fs_backend):
    result, errors = fs_backend.scan_parameters(["param1"], "/non/existent/path")
    assert result == {}
    assert len(errors) > 0

def test_scan_parameters_subset(fs_backend, temp_fs):
    params_to_find = ["param1"]
    result, errors = fs_backend.scan_parameters(params_to_find, str(temp_fs))
    
    assert len(result) == 1
    assert not errors
    p1_path = str(temp_fs / "param1")
    assert p1_path in result
    assert str(temp_fs / "subdir" / "param2") not in result
