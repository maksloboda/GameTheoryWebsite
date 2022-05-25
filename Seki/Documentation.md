# Seki

## Description

This part of the project provides code for variations of game Seki.

## Type definitions

1. PlayerIdentifier := "R" | "C" | ""
   Note: "" only exists for denoting a draw
2. GameType := "seki" | "dseki"
3. PassPolicy := "R" | "C" | "RC" | ""
4. Move := {
   int X
   int Y
   bool IsPass
   }
5. State := {
   PlayerIdentifier CurrentPlayer
   int Width
   int Height
   int[] FlattenedField
   GameType Type
   PassPolicy Pp
   int PassCount
   }
   Note: Do not specify PassCount

## Available methods

1. bool isStartStateValid(GameInfo, PlayerIdentifier)
2. GameInfo AddEvent(GameInfo, PlayerIdentifier, string move)
   Note: move should contain json of an object that has type Move
3. GameInfo joinGame(GameInfo, PlayerIdentifier)

## Calling methods

TODO

## 