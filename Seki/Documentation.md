# Seki

## Description

This part of the project provides code for variations of game Seki.

## Type definitions

1. PlayerIdentifier := `"R" | "C" | ""`

   Note: "" only exists for denoting a draw
2. GameType := `"seki" | "dseki"`
3. PassPolicy := `"R" | "C" | "RC" | ""`
4. Move := 
```
   {
   int X
   int Y
   bool IsPass
   float Value
   }
   ```
   Note: Ignore Value if not required

5. State := 
```
   {
   PlayerIdentifier CurrentPlayer
   int Width
   int Height
   int[] FlattenedField
   GameType Type
   PassPolicy Pp
   int PassCount
   }
```
   Note: Do not specify PassCount

## Available methods

1. bool isStartStateValid(GameInfo, PlayerIdentifier)
2. GameInfo AddEvent(GameInfo, PlayerIdentifier, string move)

   Note: move should contain a json of an object that has type Move
3. GameInfo joinGame(GameInfo, PlayerIdentifier)
4. Move findOptimalMove(GameInfo)

## Calling methods

Provide the following structure to stdin:

```
{
   string method
   any[] params
}
```

If return code is 0 then result of the computation will be sent to stdout as a string. Otherwise stdout will contain a string describing the error.


## 