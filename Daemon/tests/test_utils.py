import pytest
from utils import parse_range


def test_parse_range_valid():
    p_min, p_max, p_step = parse_range("0 10 0.5")
    assert p_min == 0.0
    assert p_max == 10.0
    assert p_step == 0.5


def test_parse_range_partial():
    p_min, p_max, p_step = parse_range("5")
    assert p_min == 5.0
    assert p_max is None
    assert p_step is None


def test_parse_range_empty_or_none():
    assert parse_range("") == (None, None, None)

    assert parse_range(None) == (None, None, None)


def test_parse_range_malformed():
    with pytest.raises(ValueError):
        parse_range("not_a_number")
