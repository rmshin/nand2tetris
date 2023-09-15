// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

@SCREEN
D=A
@pointer // pointer to track screen word location
M=D
(CLEAR)
    // go to fill if any key pressed
    @KBD
    D=M
    @FILL
    D;JGT

    @SCREEN
    D=A
    @pointer
    D=M-D
    A=M
    M=0 // clear screen white
    @CLEAR
    D;JEQ // stop decrement if screen all white

    @pointer
    M=M-1
    @CLEAR
    0;JMP
(FILL)
    // go to clear if no keys pressed
    @KBD
    D=M
    @CLEAR
    D;JEQ

    // SCREEN (16384) + screen size (8192)
    @24576
    D=A
    @pointer
    D=D-M
    @FILL
    D;JEQ // stop fill if screen all black

    // fill screen black
    @pointer
    A=M
    M=-1
    @pointer
    M=M+1

    @FILL
    0;JMP