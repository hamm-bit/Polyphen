This is just a rant/brainfry tab

-- Jan 19th
Comparing to Haskell, C++ allows far more explicity but at the cost of 
elegance.
The original plan of writing this compiler actually included making a
"model" using Haskell to justify the rigorousness and feasibility of
future features. However due to time limitation and my lack of 
fluency in Haskell the model is not ready at all.

The intention of this language is merely to serve as a practice over
application, since I have only taken the theory of prog langs but not
the actual process (COMP3131 compiler design). My main interest lies
in operating systems and hardware logic designs, but unarguably 
neither of these can exist if we dont have the necessary tools to 
build then, that being prog langs.

My peers have all have significantly more experience than me at 
actually building OS'es, but none of them have thought about the 
correlation of a system's performance with the compiler and the lang
design itself.



-- Jan 22nd
Example of global variable in `doc.md` shows variable shadowing
We can explicitly disallow variable shadowing for now.
However, as we go further in features, this case of shadowing should
be overcome. An example being

```
Program:
    ...
    x = 1

    # lambda expression
    parity = (\x -> x % 2) x

    ...
```

A typical lambda expression that has its argument shadowed by 
predeclared variable. 