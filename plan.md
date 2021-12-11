# DOGE

Name: Data-Oriented Game Engine

Namespace: doge

Description: Data oriented game design is encouraged, but is not fully required.

# Architecture

![image-20211211105432852](C:\Users\Lio\AppData\Roaming\Typora\typora-user-images\image-20211211105432852.png)

# Data Types

## EntityID

Contains the entity ID only.

## Entity

Contains:

- Entity ID
- SceneInfo
- Raw pointer to PCNode
- Methods for manipulating components
- Methods for manipulating parent-child nodes

## Components

Contains:

- The actual component (class defined by programmer)
- Entity ID
- Method for getting the entity

## Scene

Contains:

- Methods, i.e. Start, Update, FixedUpdate, Finish.

# Class

## Engine

Provide programmer main control of program, such as defining scenes, retrieving entities, etc.

Contains:

- Scenes
- Methods for SFMLImpl, LIC, PCNode.

## PCNode

Contains:

- Static root
- Raw pointer to parent
- Unordered set of shared pointer to children
- Methods to manipulate parent-child nodes