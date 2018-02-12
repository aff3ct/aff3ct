# Contributing Guidelines

We're really glad you're reading this, because we need volunteer developers to expand this project.

Here are some important resources to communicate with us:

  * [The official website](http://aff3ct.github.io/),
  * Send us an email to [aff3ct@googlegroups.com](mailto:aff3ct@googlegroups.com),
  * Join our [diffusion list](https://groups.google.com/d/forum/aff3ct),
  * Bugs? Report issues on [GitHub](https://github.com/aff3ct/aff3ct/issues).

## Submitting changes

Please send a [GitHub Pull Request to AFF3CT](https://github.com/aff3ct/aff3ct/pull/new/master) with a clear list of what you've done (read more about [pull requests](https://help.github.com/articles/about-pull-requests/)).

Always write a clear log message for your commits. One-line messages are fine for small changes, but bigger changes should look like this:

    $ git commit -m "A brief summary of the commit
    >
    > A paragraph describing what changed and its impact."

## Testing

We try to maintain a database of BER/FER reference simulations. Please give us some new references which solicit the code you added.
We use those references in [an automated regression test script](https://github.com/aff3ct/aff3ct/blob/master/ci/test-regression.py).
To propose new references please use our [dedicated repository](https://github.com/aff3ct/error_rate_references) and send us a pull request on it.

## Coding conventions

Start reading our code and you'll get the hang of it.
For the readability, we apply some coding conventions:

  * We indent using tabulation (hard tabs).
  * We ALWAYS put spaces after list items and method parameters (`[1, 2, 3]`, not `[1,2,3]`), around operators (`x += 1`, not `x+=1`), and around hash arrows.
  * We use the [snake case](https://en.wikipedia.org/wiki/Snake_case) (`my_variable`, not `myVariable`), classes start with an upper case (`My_class`, not `my_class`) and variables/methods/functions start with a lower case.
  * The number of characters is limited to 120 per line of code.

This is open source software. Consider the people who will read your code, and make it look nice for them.
It's sort of like driving a car: Perhaps you love doing donuts when you're alone, but with passengers the goal is to make the ride as smooth as possible.

## Documentation

At this time, AFF3CT is not as documented as we would like to be. If you are interested in enrich the AFF3CT documentation, we are open to receive suggestions.
You can also propose new pages for [the wiki](https://github.com/aff3ct/aff3ct/wiki).

Thanks,

the AFF3CT team