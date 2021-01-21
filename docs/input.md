# Input

Code to handle player input.

## Actions

```c
struct Input
{
    struct Button up;
    struct Button down;
    struct Button left;
    struct Button right;
    struct Button action;
    struct Button cancel;
    struct Button start;
};
```

Posible buttons are as follows:

* action
* cancel
* start

Directional pad:

* up
* down
* left
* right


## Button

```c
struct Button
{
    int key;
    int state;
    int action_state;
    int action_flag;
};
```
This struct holds all the date related to the button

* key: SDL ScanCode representation of a key.
* state: Holds the state of the key. If pressed state is 1, if unpressed is state is 0.
* action_state: This state is used for menus, if the key is pressed only triggers for one frame, then it turns 0.
* action_flag: Flag to keep track of the action_state.


