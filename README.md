# Spring 2022 Final Project 

## Important:

- You DO NOT need to implement GitHub Actions for the final project.  
- DO NOT attempt to push the data set for this project to Github.  
  - Make sure your data folder is in the `.gitignore` file

## Links:

- Tutorial on RapidJSON by TA Christian > [here](https://github.com/Gouldilocks/rapidTutorial) <. 
- Project Handout > [here](https://docs.google.com/document/d/1210mEIwg2rVnId4POk5gmaWFZ3mD27dts4Kwh4RTBbA/edit?usp=sharing) <.

## Coding Conventions

- ***variable and function names***

Variables and functions name shall follow the *snake case* naming style.
```c++
int variable_name;
int function_name();
```
- ***Class declaration comments***

Class declarations shall be preceded by the following comment:
```c++
/**
 * @Author(s): 
 * @filename: 
 * @date: mm-dd-yyyy
 * @description: 
 */
```
- ***Avoid global namespaces***

Using global namespaces is strictly forbidden.

**NO** ❌
```c++
using namespace std;
cout << "Hello world" << endl;
```

**YES** ✔️
```c++
std::cout << "Hello world" << std::endl;
```

- ***Short commits***

Commits must be relatively short. Ideally, must do **ONE** thing at a time. Example:

```shell
git commit -m 'Implemented member function X from Class Y'
or
git commit -m 'Fixed seg fault in function Z'
```

- ***Function declaration comments***

function declarations shall be preceded by the following comment:
```c++
    /// \param              -> 
    /// \return             -> 
    /// \description        -> 
```

Example:
```c++
    ///
    /// \param folder       -> Path to a folder withing the filesystem
    /// \return Vector      -> A set of processed JSON (Article objects)
    /// \description        -> Call "parse_json" on EACH JSON file within "folder"
    static std::vector<Article> parse_folder(const std::filesystem::directory_entry &folder);
```