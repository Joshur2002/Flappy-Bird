# Flappy Bird
- A replication of the mobile game Flappy Bird with reinforcement learning.

## Description
- Reproduced in C++
- Reinforcement-learning with Q-learning and Epsilon-Greedy algorithms.
   - Unordered map with custom hash function used for Q-table and N-table
   - Reinforcement learning algorithm with Q-learning is split into four parts:
      - Initialize: Checks whether the tuple has been registered in Q-table and/or N-table
      - Action: Decides whether to explore or exploit based off hyperparameters
      - Learn: Updates the tables with new values

## Specs
- Used Visual Studios 2022.
- Used Visual C++ 15 (2017) - 32-bit.

## Sources
- Click "Latest Stable Build" to find the different SFMLs [here](https://www.sfml-dev.org/download.php).
- Steps on how to [download and prep SFML into Visual Studios](http://gamecodeschool.com/sfml/setting-up-visual-studio-and-sfml-development-environment/).
- Steps on how to [create an SFML template in Visual Studios](http://gamecodeschool.com/sfml/building-your-first-sfml-game-project/).


