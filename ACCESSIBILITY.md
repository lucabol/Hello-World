# Accessibility Testing Checklist

This document provides a comprehensive checklist for testing and maintaining accessibility compliance in the Visual C Code Editor.

## WCAG 2.1 Compliance Status

The Visual C Code Editor is designed to meet **WCAG 2.1 Level AA** standards.

## Quick Validation Checklist

Use this checklist when making changes to `tools/editor/editor.html`:

### ✅ Keyboard Navigation
- [ ] All interactive elements accessible via Tab key
- [ ] Tab order is logical (palette → workspace → buttons)
- [ ] Enter/Space activates buttons and adds blocks
- [ ] Delete/Backspace removes focused blocks
- [ ] Escape key closes dialogs/modals
- [ ] No keyboard traps (can Tab out of all sections)

### ✅ Screen Reader Support  
- [ ] All interactive elements have descriptive `aria-label` attributes
- [ ] Regions use proper `role` attributes (`region`, `application`)
- [ ] Dynamic content uses `aria-live` for announcements
- [ ] Form inputs have associated labels
- [ ] Button purposes are clear from labels

### ✅ Visual Design
- [ ] Color contrast ratios meet WCAG AA standards (4.5:1 for text)
- [ ] Focus indicators are visible on all interactive elements
- [ ] UI works with browser zoom up to 200%
- [ ] Text remains readable when CSS is disabled

### ✅ Touch/Mobile Support
- [ ] Touch targets are at least 44x44 pixels
- [ ] Drag-and-drop has keyboard alternative
- [ ] Works on touch devices with keyboard fallback

## ARIA Structure Mapping

This mapping documents the ARIA attributes in `editor.html` and their purposes:

### Block Palette (`#palette`)
```html
<div id="palette" role="region" aria-label="Block palette">
  <button role="button" tabindex="0" aria-label="Add Include Library block">
  <button role="button" tabindex="0" aria-label="Add Print Text block">
  <button role="button" tabindex="0" aria-label="Add Return Value block">
</div>
```

### Workspace (`#workspace`)
```html
<div id="workspace" role="region" aria-label="Code workspace">
  <div aria-live="polite" aria-atomic="true">
    <!-- Dynamic block announcements -->
  </div>
</div>
```

### Code Preview (`#preview`)
```html
<div id="preview" role="region" aria-label="Code preview">
  <pre aria-live="polite" aria-atomic="false">
    <!-- Live code updates -->
  </pre>
</div>
```

### Control Buttons
```html
<button role="button" aria-label="Export generated code to hello.c">
<button role="button" aria-label="Clear all blocks from workspace">
```

### Block Delete Buttons
```html
<button class="delete-btn" aria-label="Delete this block" tabindex="0">
```

## Automated Testing

### Current Coverage
- **Integration tests** verify ARIA structure presence
- **Unit tests** validate helper functions

### Manual Testing Required
The following accessibility features require manual testing:

1. **Screen Reader Testing**
   - Test with NVDA (Windows) or JAWS
   - Test with VoiceOver (macOS)
   - Verify announcements when blocks are added/removed

2. **Keyboard-Only Navigation**
   - Disconnect mouse and navigate using only keyboard
   - Verify all features are accessible
   - Check focus visibility

3. **Color Contrast**
   - Use browser DevTools to check contrast ratios
   - Test with color blindness simulators

4. **Zoom Testing**
   - Test at 200% browser zoom
   - Verify no content is cut off or overlapped

## Manual Testing Procedures

### Keyboard Navigation Test (5 minutes)

1. **Disconnect your mouse** (to ensure keyboard-only testing)

2. **Test palette navigation:**
   ```
   1. Press Tab until focus is on "Include Library" button
   2. Press Enter to add block to workspace
   3. Verify block appears in workspace
   ```

3. **Test workspace navigation:**
   ```
   1. Press Tab to navigate to workspace area
   2. Press Tab to focus on individual blocks
   3. Press Delete to remove focused block
   4. Verify block is removed
   ```

4. **Test control buttons:**
   ```
   1. Tab to "Export to hello.c" button
   2. Press Enter to trigger export
   3. Tab to "Clear All Blocks" button
   4. Press Enter and confirm dialog with keyboard
   ```

5. **Verify focus indicators:**
   ```
   - Every focused element should have visible outline/border
   - Focus should never be lost (always visible)
   ```

### Screen Reader Test (10 minutes)

**Using NVDA/JAWS (Windows) or VoiceOver (macOS):**

1. **Enable screen reader** and navigate to editor

2. **Test landmark navigation:**
   ```
   - Use region navigation commands
   - Verify "Block palette", "Code workspace", "Code preview" regions
   ```

3. **Test button announcements:**
   ```
   - Tab to each button
   - Verify descriptive label is announced
   - Example: "Add Include Library block, button"
   ```

4. **Test dynamic content:**
   ```
   - Add a block and verify announcement
   - Remove a block and verify announcement
   - Edit block value and verify code preview updates are announced
   ```

5. **Test form controls:**
   ```
   - Tab to input fields in blocks
   - Verify labels are announced correctly
   ```

## Test Maintenance

### When to Update Tests

**Update integration tests** (`test/test_editor_integration.js`) when:
1. Adding new ARIA attributes
2. Changing element roles
3. Modifying keyboard interaction handlers

**Update this checklist** when:
1. Adding new interactive features
2. Changing UI structure
3. Implementing new accessibility patterns

### Integration Test Coverage

The automated tests verify:
- Minimum 8 `aria-label` attributes present
- Palette has `role="region"` with label
- Workspace has `role="region"` with label  
- Code preview has `aria-live="polite"`
- All buttons have proper roles

## Resources

### Testing Tools
- **NVDA**: Free screen reader for Windows
- **JAWS**: Commercial screen reader (trial available)
- **VoiceOver**: Built-in macOS screen reader (Cmd+F5)
- **axe DevTools**: Browser extension for accessibility testing
- **WAVE**: Web accessibility evaluation tool

### Standards & Guidelines
- [WCAG 2.1 Guidelines](https://www.w3.org/WAI/WCAG21/quickref/)
- [WAI-ARIA Authoring Practices](https://www.w3.org/WAI/ARIA/apg/)
- [MDN Accessibility Guide](https://developer.mozilla.org/en-US/docs/Web/Accessibility)

### Color Contrast Tools
- [WebAIM Contrast Checker](https://webaim.org/resources/contrastchecker/)
- Chrome DevTools: Inspect > Accessibility > Contrast

## Future Enhancements

### Optional Automated Testing
For stronger accessibility validation, consider adding:

1. **Headless browser tests** with Puppeteer/Playwright:
   ```javascript
   // Example: keyboard navigation test
   await page.keyboard.press('Tab');
   await page.keyboard.press('Enter');
   const blockCount = await page.$$eval('.block', blocks => blocks.length);
   ```

2. **axe-core integration**:
   ```javascript
   // Example: automated accessibility audit
   const results = await page.evaluate(() => axe.run());
   assert.strictEqual(results.violations.length, 0);
   ```

These are **optional** and not required for the zero-dependency design.

## Reporting Issues

If you discover accessibility issues:

1. **Document the issue:**
   - What feature is inaccessible?
   - Which assistive technology (screen reader, keyboard, etc.)?
   - Expected vs. actual behavior

2. **Check against WCAG:**
   - Which WCAG criterion is violated?
   - What level (A, AA, AAA)?

3. **Propose a solution:**
   - What ARIA attribute/pattern would fix it?
   - Reference WAI-ARIA best practices

4. **Create an issue** with the "accessibility" label
