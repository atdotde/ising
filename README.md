# Explore a phase transition on the computer

Here are some suggestions for things you could investigate in a numerical simulation of a the 2D Ising model to gain more intuition about phase transitions.
To get you started, you are provided with a C program that implements the Monte Carlo algorithm discussed in the lecture for the 2D Ising model. You can find the program on
https://github.com/atdotde/ising
You will find that you will spend quite some time to wait for that program to reach anything like equilibrium. This is why it is written in C (we could have also chosen other properly compiled languages like C++) and not in a scripting language like Python you might be more familiar with. But an order of magnitude in execution speed can make the difference between waiting for some minutes versus waiting for some hours. You will find that in the end C is not so much different from other related programming languages and the web will help you to get up to speed.

## Preparation
You need to make sure you have a C compiler installed on your computer. This could for example be gcc (the GNU C compiler) or clang (the C compiler coming with llvm), both available as open source software for pretty much any computer. If you are on a Linux computer, chances are you already have gcc installed. If you are on a Mac, you need to install Xcode and the “developer command line tools” available free of charge from the AppStore. If you are on Windows, there are also possibilities but I am not familiar with those. Use the web.
Then, life gets easier by using the git version control system. Again, on Linux it’s probably already there, on Mac it came with the developer tools and on Windows you are on your own. In a terminal window, run

```git clone https://github.com/atdotde/ising```

and it will create a directory called ising that contains all the files you need. Go to that directory and run

```make```

and it will compile a binary that you can run with

```./ising```

You should make sure your window is big enough to show the whole grid. You can now open ising.c in your favourite editor and modify it according to your liking. You probably want to start by changing the constants by a bit.
My recommendation is to use the integrated development environment QtCreator. This contains an editor as well as a debugger which will make your life much easier. Again, this is open source software and you should be able to install it though your package manager (on a Mac that would for example be home brew). Once started, you can open a project and select the ising.pro file.
Another tool you will need is a program that can turn files containing numbers into nice plots. Again, an open source option would be to use gnuplot. The program as it is writes to a file magnetization and in gnuplot you can plot it with
plot “magnetization”

## Finding the phase transition
A simple first task is to determine the value of the coupling J at which the phase transition happens. To this end, start with a strong external magnetic field to align the spins and then slowly lower it by multiplying it every K steps by 0.99 while watching the magnetization m. If J is above the phase transition, there will be a remaining magnetization even when the external field is essentially gone.
You will need to find a good value for K such that the change in the magnetic field is almost adiabatic (the spins should have enough time to equilibrate for each value of J) while you balance this with your patience in waiting for results.
Unfortunately, the closer you get to the phase transition, the longer it takes to equilibrate (a further project would be to quantify this relation). So in finite time you will never reach the actual critical value. But what you can do is to plot the remaining magnetization as  a function of J and extrapolate that relation to m=0.
Note that most of the computation time is spent on printing out the grid. To speed it up, you should turn off this output and just use the values of m.
Just for your reference, the critical value is ln(1+sqrt(2))/2 but you should only evaluate this into a decimal number once you have a good guess from your simulations!

## Find the correlation length
Once you determined the critical coupling you could for example determine the correlation length: You look at how much two spins are correlated as a function of their distance. You compare the expectation of grid[x][y] * grid[x+l][y] to the product of the two expectations. You should find that that decays exponentially like exp(-c*l). 1/c is called the correlation length and you could study how that scales with J-Jcritical. Spend some thoughts on how to effectively determine c in the simulation.

## Finite size scaling
Of course, our grid is finite and a proper phase transition can only take place on an infinite grid. Otherwise, if we wait long enough every spontaneous magnetization will disappear eventually. You can study this by investigating how your previous results depend on the grid size and possibly take extrapolations.

## Speeding things up
You can try to play with the update rule of the Metropolis algorithm to make the system equilibrate faster. For example you could occasionally try to flip larger regions than just individual spins. Just make sure you don’t break detailed balance and the proof of convergence to thermal equilibrium.
