

Function syntax in Polyphen is identical to `Haskell`
function definition example:

```
Program:

    square :: Int -> Int
    square a:
        return a * a

    # outputs 25
    return square 5

```
In the future, implicit type inference will also be introduced
So we can save the explicit declaration of types before heading
As well as supporting inline lambda expressions


However this language is not purely functional, so we do not
have pure functions. For example, we can have global variables:

```
Program:
    ...

    let a = 3

    # the following constant assignment may be added in future
    # let b := 3

    # 
    square :: Int -> Int
    square a:
        return a * a

    ...
```


