# DOGE

Name: Data-Oriented Game Engine

Namespace: doge

Description: Data oriented game design is encouraged, but is not fully required.

# Architecture

![image-20211211205543684](C:\Users\Lio\AppData\Roaming\Typora\typora-user-images\image-20211211205543684.png)

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

## GameLoopFunctions

Contains:

- Start
- Update
- FixedUpdate
- EarlyUpdate
- Finish

# Class

## Engine

Provide programmer main control of program, such as defining scenes, retrieving entities, etc.

Contains:

- Scenes
- Methods for IOBus, LIC, PCNode.

## PCNode

Contains:

- Static root
- Raw pointer to parent
- Unordered set of shared pointer to children
- Methods to manipulate parent-child nodes

## IOBus

Contains:

- Input and output functions
- [SFML]([SFML (sfml-dev.org)](https://www.sfml-dev.org/)) is used

# Extensions

## Physics

Contains:

- Static, kinematic, dynamic rigid body simulation
- [Box2D](https://box2d.org/) is used
- **SHOULD NOT** be used with the parent-child system

# Built-In Components

## Shapes

Includes:

- RectangleShape
- CircleShape
- ConvexShape

Usage:

- Rendered and drawn by engine on screen
- Top-left is origin (0, 0), except ConvexShape

## Colliders

Includes:

- RectangleCollider
- CircleCollider
- ConvexCollider

Usage:

- Use with RigidBody to simulate collisions
- Centroid is origin (0, 0), except ConvexCollider