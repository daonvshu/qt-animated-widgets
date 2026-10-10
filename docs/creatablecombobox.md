# Creating combo-box options

`CreatableComboBox` extends `SearchComboBox` with a Create action at the bottom
of its popup. Filtering, highlights, custom popup styles and item delegates work
as they do for `SearchComboBox`. Creation also works when the model is empty.

The control emits `createRequested(text)`; it does not insert rows or save data.
The application can validate the request, start an asynchronous operation, or
insert and select the new option immediately:

```cpp
connect(combo, &animatedwidgets::CreatableComboBox::createRequested,
        combo, [combo](const QString& text) {
    combo->addItem(text);
    combo->setCurrentIndex(combo->count() - 1);
});
```

The candidate is trimmed. Blank text, text shorter than `minimumCreateLength`,
and exact matches to existing options do not offer creation. Duplicate matching
uses `creationCaseSensitivity`, which defaults to case-insensitive matching.
Headings and separators do not count as options; disabled options still prevent
duplicate creation. The model's active root and display column determine the
options checked for duplicates.

`creationEnabled` controls the action. `createText` is its label template, with
`%1` replaced by the candidate. `creationCandidate()` returns the currently
eligible text, or an empty string. Read-only/disabled editors, unacceptable
validator input and active IME composition suppress creation.

Enter requests creation when no existing result is highlighted, or when the
Create action is selected. Up/Down navigate results and the Create action.
Escape and focus loss dismiss the popup without creating an option. The popup
closes before `createRequested` is emitted, so a handler can open a dialog safely.
Selection remains under application control; a rejected request leaves the
previous selection in place.

The action uses `HoverButton`. During `popupAboutToShow`, `createButton()` returns
the popup-owned button so its icon, colors, font and size can be customized.
The pointer is only valid for that popup's lifetime. Customization does not
change the popup's list or body style.
