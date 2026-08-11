# MMFoundation

This project is a small hobbyist attempt to build a C library that follows, as closely as possible, the data types and programming style of Apple’s Foundation framework.

The goal is to make it easier to port console-based scripts originally written with Foundation-style APIs to other platforms and environments. The idea is to preserve the overall feel of Foundation while keeping the implementation in plain C.

To keep the library recognizable and familiar to people coming from Apple’s ecosystem, the class and object names use the same conventions as Foundation, but with NS replaced by MM. Subclasses are in the same .h and .c files of their parents

It is important to note that this is not a true object-oriented implementation. Since the project is written in C, it cannot provide full Objective-C-style object semantics. Instead, it offers a Foundation-like API and naming style using plain C structs and functions.

Similarly, features such as autorelease pools or automatic reference counting will not be implemented for the moment. Memory management will remain completely under the control of the programmer, in a pure C style.

In short, this library is an experiment in creating a lightweight, portable Foundation-like toolkit for C, inspired by the design of Apple’s Objective-C APIs.

For now, only the classes and methods strictly necessary for my own projects have been implemented. Anyone who wants to contribute, improve, or extend the library is more than welcome to do so.

The project license is described in the License file.
