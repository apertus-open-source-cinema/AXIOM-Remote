import sys
from pathlib import Path

# Add parent directory to Python path so modules can be imported
sys.path.insert(0, str(Path(__file__).parent.parent))
