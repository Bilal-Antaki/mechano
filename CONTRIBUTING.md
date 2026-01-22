# Contributing to Mechano

Thank you for your interest in contributing to the Mechano robot project!

## Code of Conduct

Please be respectful and constructive in all interactions.

## How to Contribute

### Reporting Bugs

1. Check if the issue already exists
2. Create a new issue with:
   - Clear description of the problem
   - Steps to reproduce
   - Expected vs actual behavior
   - System information (ROS2 version, Ubuntu version)

### Suggesting Features

1. Open an issue with the "feature request" label
2. Describe the feature and its use case
3. Discuss implementation approach

### Pull Requests

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-feature`
3. Make your changes
4. Run tests: `colcon test`
5. Commit with clear messages
6. Push and create a pull request

## Code Style

### C++

- Follow the [ROS2 C++ Style Guide](https://docs.ros.org/en/rolling/The-ROS2-Project/Contributing/Code-Style-Language-Versions.html)
- Use clang-format with the provided `.clang-format` file
- Use meaningful variable and function names

### Python

- Follow PEP 8
- Use type hints where appropriate
- Document functions with docstrings

### Launch Files

- Use Python launch files (not XML)
- Include clear descriptions for arguments
- Follow the pattern established in existing launch files

### URDF/Xacro

- Use xacro macros for reusable components
- Document dimensions and units in comments
- Keep files modular (one file per component type)

## Testing

- Add unit tests for new functionality
- Ensure existing tests pass
- Test on both simulation and real hardware if possible

## Documentation

- Update README files when adding features
- Document new parameters and launch arguments
- Keep the docs/ folder up to date

## Commit Messages

Use clear, descriptive commit messages:

```
feat: Add battery monitoring to mechano_node

- Subscribe to battery voltage topic
- Publish low battery warnings
- Add LED indication for battery level
```

Prefixes:
- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `refactor:` Code refactoring
- `test:` Adding tests
- `chore:` Maintenance tasks

## Questions?

Open an issue or start a discussion. We're happy to help!
