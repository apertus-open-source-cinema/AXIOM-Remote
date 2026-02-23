# Contributing

## Running tests

We use pytest for unit tests. To run the tests locally:

* Create and activate a virtual environment (recommended):

```bash
python3 -m venv .venv
. .venv/bin/activate
pip install pytest
```

* Run tests with the venv's python:

```bash
.venv/bin/python -m pytest -q
```

* Or use the provided Makefile target (it will use the venv python if available):

```bash
make test
```

If you prefer not to use a venv, ensure `pytest` is installed for the Python interpreter you plan to use and run:

```bash
python -m pytest -q
```
