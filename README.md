# Solitaire in C
Written by Nathaniel Graves

Actions:

  ```d```: Draw 3 cards from the deck. If the deck is empty, this action will return the cards in the draw pile to the deck.
  
  ```p [stack]```: Move a card from the draw pile onto one of the stacks. Stacks are numbered from left to right, starting with 1.
  
  ```m [stackA] [cutoff] [stackB]```: Move cards from one stack to another. ```cutoff``` indicates how many cards should stay in place.
  
  ```f [stack]```: Move a card from one of the stacks to the foundation. To move a card directly from the draw pile, set ```stack``` to 0.
  
  ```q```: Exit the program.

There is also the option to use ```1```, ```2```, ```3```, or ```4``` instead of ```d```, ```p```, ```m```, and ```f```.

Make sure not to have anything except spaces and line breaks between commands.
