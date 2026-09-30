# Code Style and Organization

- Follow the repository's `.clang-format` for general C++ formatting. Keep the existing style in files you edit; avoid unrelated reformatting.
- Put a constructor definition's opening brace on its own line after the initializer list. Keep the opening brace of ordinary function definitions on the signature line. Apply the constructor exception manually after formatting.
- Leave a blank line between function definitions. Within longer functions, separate distinct steps with blank lines, such as widget creation, configuration, signal connections, and state updates.
- Group related declarations and member variables with blank lines. Keep getter/setter pairs together, and separate different property groups, layout objects, transitions, colors, and flags where that improves readability. Avoid both dense uninterrupted blocks and gratuitous blank lines.
- Use lowercase, concatenated source filenames without underscores. Keep each class in its own appropriately named files, with its header and source in the same directory under `src/animatedwidgets/<responsibility>/`; place button types under `src/animatedwidgets/buttons/`.
- Put shared state management and layout behavior in a base class only when they are genuinely common. Put reusable drawing or calculation helpers in utility classes with static methods; do not add free global helper functions. Keep each concrete widget's implementation explicit about the properties and behavior it actually supports.
- Follow nearby comment conventions. Add comments only where they clarify non-obvious behavior, and keep code, comments, and commit messages free of conversational language.
- Keep Qt 5 and Qt 6 compatibility when changing public widgets, examples, or Designer plugins.
