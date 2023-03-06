# Codie - C++ library

## Purpose
This is a manifestation of my preferred style of C++20 programming. Decided to put
it on github for my own convenience. It is designed in conjunction with some of my 
own tools and snippets, so I can quickly try out design ideas, as well as writing
some useful tools. At the start of this project (Summer 2022) the code was spread 
across several private code-bases and repositories from the last 3+ decades.
I intend to pick the parts I use the most and simply use it when I get an idea or 
challange. 

Sticking to some (good?) habbits regarding documentation, 
I will summarize the included parts...

For my own use! No batteries included!

## Content

The library is intended as a mostly header ***static*** library. Few people realize
that C++ does not define any ABI well enough to actually support dynamic linkage,
those (`*.dll` | `*.so`) come from the oprating systems and vanilla **C**. This 
is important to remember, and also, keep the linkable part small!

Included the folloing parts so far:

| Header                   | Description                    |
|:-------------------------|:-------------------------------|
| **`codie/common.hpp`**   | Common parts used in API's     |
| **`codie/item.hpp`**     | Base for my switchables...     |
| **`codie/single.hpp`**   | My take on singleton...        |
| **`codie/islist.hpp`**   | Simple ***intrusive*** list    |
| **`codie/slist.hpp`**    | The missing slist...           |
| **`codie/chronos.hpp`**  | Helpers for performance checks |
| **`codie/rand.hpp`**     | Short-cuts to randomisation    |

## Common ...

The types and constructs I almost always use to write API's and coding. I prefer
to use my own names on things and to have them collected in one place. So, when
I ask myself, *'Can I use this type in an API?'*, the answer is *'If it's part of common!'*. Well, it's a simplified way of picking a style ...

## Item ...
Based on the `constexpr` hashing in common, I've designed a way to dispatch 
different events and messages that rely on this. It's a working replacement 
for enumeration in cases where allocation of sequence-number is not really
feasable. So, instead of defining an enum of event-types (and failing int/enum)
I use hashes that are compile-time constants and thus switchable.

The example-code below, note that `UID("SomeText")` is used as ***compile-time*** constant!

```c++
struct event_t {
  static const hash32_t KEYPRESS   = UID("KeyPress");
  static const hash32_t MOUSEMOVE  = UID("MouseMove");

  const hash32_t event;
  union {...};
};

void event_handler( event& e ) {
  switch( e ) {
  case event_t::KEYPRESS: DoKeyPress(e); break;
  case UID("MouseMove"): DoMouseMove(e); break
  }
}
```

The **Item** - class is the base of my event- and message -solutions from the past. 
I have been using this in many projects, the most modern variants relie on the above 
hash-constant concept.. I will attempt to import the best of those into this library.
However, there are som many variants it's going to take a while to clean up  ...

## SList ...

The missing `slist<>`, is my take on a singly-linked list, which is still missing
in the standard C++ libraries. The `std::forward_list<>` is in my view a mistake 
that could have been avoided. The ***intrusive*** `islist<>` and `slist<>` complement eachother and provide an highly efficient list that can be used as both **FIFO** and 
**LIFO** situation. The `slist<>` itself has a feature I have relied on many times, 
allowing insertion/extraction of list-elements onto a smart-pointer. My first variant on 
this, from the 90's, had it's own smart-pointer implementation and has proven to be 
incredibly usefull for message-queuing, complex streaming and reactive-like scenarios. 

## Single ...

My take on ***singleton*** - pattern. I strongly believe in properly bound lifetime
of objects. This differs from common singleton implementation by ***demanding***
the binding of lifetime to a scope you have control over! Typically this means
`main()`.

```c++
class App : NoCopy {
public:
  using one = single<App>;
};

int main(int,char**) {
  App::one _app_inst_(...whatever...);
  App &app = MyApp::one::inst();
  return app.Main();
}
```

The behavior is of course different in other ways. For instance if you access
`App::one::inst()` before it was created, or after it's destroyed, an exception
is thrown. Also, a second instantiation of `App::one` will throw an exception.
It's up to the coding of `App` to choose is that also goes for a regular instance.
But then, again, it may be a relevant case to separate a global `App::one`
from regular instances.

## Chronos
I like to keep track of the performance. I have a growing number of helpers
to make this easy...

## Rand
Randomization shortcut's I've grown accustomed to...