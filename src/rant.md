This is just a rant/brainfry tab

-- Jan 19th
Comparing to Haskell, C++ allows far more explicity but at the cost
of elegance.
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

___

-- Jan 22nd
Example of global variable in `doc.md` shows variable shadowing
We can explicitly disallow variable shadowing for now.
However, as we go further in features, this case of shadowing should
be overcome. An example being

```Polyphen
Program:
    ...
    let x = 1

    # lambda expression
    let parity = (\x -> x % 2) x

    ...
```

A typical Haskell-style lambda expression that has its argument 
shadowed by predeclared variable. 

___

-- Jan 30th
Var environment is being constructed currently in tokenization step.
Originally it is thought that filling the env table should be the job
of the evaluator.
Since I had no prior experience of actually building a lexer, I went 
and had a look at other languages' compilers. Polyphen borrows
syntaxes from Haskell, but with the looks similar to JS (which I dont
use or know well lol).

Albeit the language currently does not support implicit type inference,
the underlying structure already has fundamental traits of it. The 
environments used do not differentiate base types, so type assignments
are not necessary. I personally prefer this just for the sake of testing
my implementation skills with knowledge obtained from this past term
(and maybe in future, OOP with parametric polymorphism).

Of course, from a performance angle this is rather hideous in comparison
to a classic imperative language C where everything has a predefined 
type. You do not have to spend computation power trying to figure out the
type when substituting via environment, nor having to setup huge
background scheduler supporting garbage collector to scower through the
environment finding variables and ADTs (in future!) that should be dumped.

___

-- Feb 2nd
I need to run `coom` everytime a token is lexed, in a lot of branched 
cases I need to pad over ' ' space. (`pad`)
If only C++ has something equivalent to Monad `>>=` or `bind` operator...

(bruh moment setup)
```cpp
===================================================================
...

template<typename T>
class Monad {
public:
    Monad(Const T& val) : val(val)
    {
    }

    template<typename F>
    auto operator>>=(F&& f) const {
        // albeit `return` in c++ is very different from haskell
        // All programs will then have to maintain a full map of
        // variable environment.
        // The performance deficit of the above map is definitely
        // not worth if we are simply implementing it for Monad,
        // without a full framework of FP paradigm support.
        // 
        return f(v);
    }

private:
    T val;
};

// Terrible shit

...
===================================================================

```

___

-- Feb 8th
Assignment operators will take the job of substituting values into the
tokens. The values for substitutions are unprocessed strings.

___

-- Feb 12th
When constructing parse tree, the value substitutions will be turned into
an environemnt then. Tokenization will purely hold the job of seperating
the tokens from the content.
Rewritten the shittily implemented tokenization function with LUT (I was mislead).

___

-- Feb 28th
Since the language will be more functional biased, the values were defaulted to immutable (like Rust). This is not necessarily easier than a mobile implementation, since now any STL data structures will be defaulted to some immutability based.
In hindsight, this is good since the language will carry innate concurrency for multithreaded execution when configured. (Ugh, concurrency support will be a thing in 2 years... I think... At this point this language is looking more and more like Rust). However it will be also be different, since it will carry some garbage-collection ideas over from Haskell, C-- specifically. An example will be (possibly) STM for low contention systems.

