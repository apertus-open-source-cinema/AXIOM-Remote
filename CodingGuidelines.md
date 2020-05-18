# Coding guidelines

In order to help everyone read and easily understand the code, it must adhere to certain style rules:

- Variables or attributes should start with a lower case letter and methods, classes, or interfaces with an upper case letter.

  <sub>Example:</sub>

  ```cpp
  int frameCount

  void Process()

  class FrameProcessor
  ```

- Private/protected member variables (attributes) should be prefixed with an underscore ( _ ).

  <sub>Example:</sub>

  ```cpp
  float _posX
  ```

- if public member variables (attributes) are present, e.g. struct are public by default, then the names have to start with capital letter.

  <sub>Example:</sub>

  ```cpp
  struct Icon
  {
        uint16_t Width;
        uint16_t Height;
        ...
  };
  ```

- Interfaces should be prefixed with an upper case I.

  <sub>Example:</sub>

  ```cpp
  class IProcessor
  {
  public:
        virtual void SomeMethod() = 0;
        ...
  };

  class DataProcessor : public IProcessor
  {
  public:
        void SomeMethod() override
        {
                ...
        }
        ...
  };
  ```

- IF statements should always use curly braces - even for single lines.

  <sub>Example:</sub>

  ```cpp
  if (condition)
  {
        do_something();
  }
  ```
