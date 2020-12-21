# Day 13, Part 2 #

## Requirements ##

Analyze the list of Bus IDs to determine when they can fit into a schedule so that bus_id 0 happens at time 0, bus_id 1 happens at time 1, etc. When there are x values in the list of IDs, it doesn't matter what happens at those times.

Return the value of the minimum timestamp for that the schedule holds true. It will be over a trillion.

### Input Format ###

Two lines of input.
First line: a number corresponding to the earliest timestampt that I could depart a bus. Ignore that for Part 2
Second line: a comma-separated list of numbers (for Bus ID) or `x` values for that time spot doesn't matter.

### Output Format ###

Number - the ID of the bus multiplied by the minutes we have to wait to board the bus.

## Test Cases ##

### Given Test Cases ###

There are five test cases given:
* [1](../data/test_cases/day13_test1.txt)
* [2](../data/test_cases/day13_test2.txt)
* [3](../data/test_cases/day13_test3.txt)
* [4](../data/test_cases/day13_test4.txt)
* [5](../data/test_cases/day13_test5.txt)

### Custom Test Cases ###

Hoping I don't need to make up a custom test case.

## Approach ##

I will be doing this working through the Chinese Remainder Theorem. I found a good [Youtube Video](https://www.youtube.com/watch?v=2-tdwLqyaKo) which explains it straightfoward, so I will be working on implementing that algorithm.

There are also two Youtube Videos for the Extended Euclidean Algorithm for Module inverses that I used. [Video 1](https://www.youtube.com/watch?v=shaQZg8bqUM)  [Video 2](https://www.youtube.com/watch?v=fz1vxq5ts5I).


### Input Parsing ###
* Parse the input using my function for lines based on commas
* Skip the first line
* Return a list of Bus IDs for the second line, with 0's in place of where the x's are.

### Creating the Set of Equations to Solve ###

Let's call the timestamp we are looking to solve *t*.  

`Bus_ID[0]` will arrive at *t*, so we can define the equation `t = 0 % Bus_ID[0]`  

For subsequent items where `Bus_ID[n]` is not the `x` value, we know that it needs to arrive at *n* seconds after *t*. So, we can create the equation `(t + n) = 0 % Bus_ID[n]`.  
However, to use the Chinese Remainder Theorem to solve for *t*, we need to subtract the *n* from both sides. This gives us `t = (0 - n) % Bus_ID[n]`. If `(0 - n) % BUS_id[n]` is negative, we just need to add `Bus_ID[n]` to it to have a positive value between `0` and `n-1`.

The resulting set of these equations will be ready for the Chinese Remainder Theorem.

### Euclidean Algorithm for Module Inverse ###

To solve `m*x = 1 mod n` for x:  

Going to work down a series of equations of the form `a = b*c + d`

* Set up the first equation (i=0) so that `a[0]=n` and `b[0]=m`, and solve for the `c` and `d` values. (`c=a/b` and `d=a%b`).  
* While `d[i]` is not 1:  
    *set `a[i+1]=b[i]` and `b[i+1]=d[i]` and repeat.


Now we need to work backwards up the tree with *j* going from the final *i* to *1*

I had to do a whole bunch of math on paper multiple times to figure this out, so it's not going to be retyped here.

Working back up, we need to find the coefficients before the `a[j]` and `b[j]` terms so that `1=coef_a*a[j] + coef_b*b[j]`  
Solving the lowest equation, `coef_a=1` and `coef_b=-1*c[j]`.  
For lines above that, `coef_a[j]=coef_b[j+1]` and `coef_b[j]=coef_a[j+1]-(coef_b[j+1]*c[j])`

Doing this repeatedly, we end up with 1 defined in terms of `a[0]` and `b[0]`: `1 = coef_a[1]*a[0] + coef_b[1]*b[0]`.  
Since we have started `a[0]=n` (our modulus), that term drops out for the modul addition. *coef_b[1]* is the inverse that we are looking for.

### Chinese Remainder Theorem Algorithm ###

We have a system of *k* equations of the form `t = n[i] % Bus_ID[i]` with all the Bus_IDs relatively prime.  

Set *m* = the products of all of the Bus_IDs. (Bus_ID[0] * Bus_ID[1] * ... * Bus_ID[k])  

Define `M[0]` through `M[k]` to be `(m/Bus_id[k])`.  

Define `y[0]` through `y[k]` so that `M[k]*y[k] = 1 % Bus_ID[k]`. This will involve using the Euclidian Algorithm since we have large numbers.

Result is `(n[0]*M[0]*y[0] + n[1]*M[1]*y[1] + ... + n[k]*M[k]*y[k]) % m`

### Main Loop ###
* Parse the input as described above, returning the list of Bus_IDs
* Create the Set of the Equations to Solve as Described Above.

## Things I learned ##

Application and implementation of a Chinese Remainder Theorem algorithm.

Working through the implementation of the Extended Euclidean Algorithm. I could have done this with a simple trial and error in less time than it took to understand that algorithm.

