# Coding guidelines

In order to help everyone read and easily understand the code, it must adhere to certain style rules:

- Private/protected member variables (attributes) should be prefixed with an underscore ( _ ).

  Example:

  ```cpp
  float _posX
  ```

- if public member variables (attributes) ar present, e.g. struct are public by default, then the names have to start with capital letter.

  Example:

  ```cpp
  struct Icon
  {
        uint16_t Width;
        uint16_t Height;
        ...
  }
  ```

- Interfaces should be prefixed with an upper case I.

  Example:

  ```cpp
  class IProcessor
  {
  public:
        virtual void SomeMethod() = 0;
        ...
  }

  class DataProcessor
  {
  public:
        virtual void SomeMethod() override
        {
                ...
        }
        ...
  }
  ```

- Variables or attributes should start with a lower case letter and methods, classes, or interfaces with an upper case letter.

  Example:

  ```cpp
  int frameCount

  void Process()

  class FrameProcessor
  ```

- IF statements should always use curly braces - even for single lines.

  Example:

  ```cpp
  if (condition)
  {
        do_something();
  }
  ```
