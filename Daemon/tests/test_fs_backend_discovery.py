from fs_backend import FSBackend
from pathlib import Path


def write_file(p: Path, name: str, content: str):
    f = p / name
    f.write_text(content)


def test_discover_and_load_parameters(tmp_path):
    base = tmp_path / "params"
    base.mkdir()

    # Create param dir p1 with files
    p1 = base / "p1"
    p1.mkdir()
    write_file(p1, "value", "42")
    write_file(p1, "ty", "int")
    write_file(p1, "writable", "1")
    write_file(p1, "range", "0 100 1")
    write_file(p1, "map", "a\nb\nc")

    # Create param dir p2 with missing files
    p2 = base / "p2"
    p2.mkdir()
    write_file(p2, "value", "on")

    # Create a directory under raw that should be skipped
    raw = base / "raw"
    raw.mkdir()
    raw_p = raw / "p3"
    raw_p.mkdir()
    write_file(raw_p, "value", "99")

    fb = FSBackend()
    db, errors = fb.scan_parameters(None, str(base))

    assert not errors
    assert "p1" in {Path(k).name for k in db.keys()}
    assert "p2" in {Path(k).name for k in db.keys()}
    assert all(Path(k).name != "p3" for k in db.keys())

    # check loaded data for p1
    p1_key = [k for k in db.keys() if Path(k).name == "p1"][0]
    d = db[p1_key]
    assert d["type"] == "int"
    assert d["writable"] is True
    assert d["range"] == "0 100 1"
    assert d["map"] == ["a", "b", "c"]

    # p2 defaults
    p2_key = [k for k in db.keys() if Path(k).name == "p2"][0]
    d2 = db[p2_key]
    assert d2["type"] is None
    assert d2["writable"] is False
    assert d2["map"] is None
