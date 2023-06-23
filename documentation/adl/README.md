# Asset Definition Language Version 1
This document is the manual for version 1 of the *Asset Definition Language* (*ADL*) which is used to describe audial, haptical, visual aspects of games.


## Types
*ADL* knows six basic types *List* and *Map*, *Boolean*, *Number*, *String* and *Void*.

The type *Void* has a single value *void* which is represented in the language by the literal `void`.
The type *Boolean* has two values *true* and *false* which are expressed in the language by the literal `true` and `false`, respectively.
The type *Number* represents both 2-complement integer numbers as well as floating-point numbers.
The type *String* represents UTF-8 strings. The language knows single-quoted and double quoted strings.
Single quoted strings are opened and closed by the single quote `'` and double quoted strings are opened and closed by the double quote `"`.
To type a single quote in a single-quoted string, preceed it by a backslash.
To type a double quote in double-quoted string, preceed it by a backslash. 
A double backslash is interpreted as a single slash.

The type *List* represents sequences of values the six types.
A *List* literal is opened by a left square bracket `[` and is closed by a right square bracket `]`.
The literals of the values are written between these brackets. Two consecutive literals are separated by a comma `,` inbetween them.
The literal of the last element can also be followed by a comma `,`.


The type *Map* represents a set of mappings from names to values of the six types.
A name is a sequence of UTF-8 characters of a particular format.
A *Map* is opened by a left curly bracket `{` and is closed by a right curly bracket `}`.
The literals of the mappings are written between these brackets. Two consecutive mappings are separated by a comma `,` inbetween them.
The literal of the last element can also be followed by a comma `,`.
A mapping consists of a name, followed by a colon `:`, followed by the literal of the value.

Remark: The *ADL* does not impose restrictions on the range and precision of values of type *Number* nor on the length of values of type *String*.

---
**Example**
The folowing *ADL* program defines a map which maps the names `text` and `action` to the values `[ 'Hello,',  'World!' ]` and `action : 'Print'`, respectively.
```
{ text : [ 'Hello,',  'World!' ], action : 'Print' }
```
The value `[ 'Hello,',  'World!' ]` defines a list with two string elements.
---


If two or more mappings from the same name in a map are specified, then the syntactically last defined mapping take precedence.

---
**Example**
The folowing *ADL* program defines a map in which maps the names `x` twice. First to the number `0` and second to the number `1`.
```
{ x : 0, x : 1 }
```
The effective *Map* value defined here hence
```
{ x : 1 }
```
---

