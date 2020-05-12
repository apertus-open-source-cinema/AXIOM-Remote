In order to help everyone read and easily understand the code, it must adhere to certain style rules:

- Private/protected member variables (attributes) should be prefixed with an underscore ( _ ).

        e.g. float _posX
        
- if public member variables (attributes) ar present, e.g. struct are public by default, then the names have to start with capital letter.

        e.g. Icon struct -> Width, Height etc.
        
- Interfaces should be prefixed with an upper case I.

        e.g. IProcessor
        
- Variables or attributes should start with a lower case letter and methods, classes, or interfaces with an upper case letter.

        e.g. int frameCount, void Process(), class FrameProcessor
        
- IF statements should always use curly braces - even for single lines.

       e.g.
       if (condition)
       {
          do_something();
       }
