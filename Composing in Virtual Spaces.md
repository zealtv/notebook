---
title: Composing in Virtual Spaces
date: 2022-03-21
lastmod: 2022-03-21T17:52:01+11:00
draft: true
---
```
These notes were written for a presentation to be given at The Victorian College of the Arts, March 23rd 2020.

Duration: 1hr + 30m Q&A
```

## Who am I?

* Audio-visual Artist*
* Composer 
* Software Developer
* Animator
* Interested in extending conceptions and applications of music
* Pro Frank Zappa
* Anti Lars Ulrich
<!--- --->
 
  Also:
  * PhD Candidate at RMIT University
  * Max Certified Trainer
  * Director of Technology at Playable Streets
<!--- --->

Some examples of my previous work are [here](https://zeal.co/projects/luminesce),[here](https://zeal.co/projects/axis),[here](https://zeal.co/projects/obstinate).  

---
## Music+

### What the hell is music anyway? 

Music infamously evades definition.   Perhaps the most often cited definition is from Edgard Varèse:

>Music is organised sound.

Though it doesn't take long to think of examples of sound that is organised, but usually not considered music.  Think industrial noises, or the warning beeper on a backing up truck. And there are many examples of music that challenge the definition, such as aleatoric music.

Instead of asking  *What is Music?*, what if we instead consider *What is **Musical**?*

To ruthlessly synthesise  musicologist [David Huron](https://mitpress.mit.edu/books/sweet-anticipation).

>Musicality is an aesthetic sense (qualia) elicited by the subconscious recognition of temporal patterns.
  
We use our past musical experiences to build probabilistic mental models to answer the following question from moment to moment:
*Given what has happened in a piece of music thus far, and the current context, what do we expect will happen next?*

✅ brain rewards itself

❌ brainstem reaction (arousal + positive valence)

The downstream effect is that **Predictability creates musicality.   Surprise adds excitement.** If the piece is too predictable we get bored.  If it is too difficult to predict, it doesn't sound musical.
  
Interestingly - this view of musicality is built atop predicting events in time, it is not constrained to sound.  Theoretically, other time-based artforms such as dance, poetry (*i.e. rap*), and video, could elicit musical experience.  As could other predictable  natural phenomena such as a leaf falling from a tree, or a rock skipping on a lake.

So let's define **Music+**  as simply *patterns in time*.  This can encapsulate composing, arranging, improvising, and performing. We often see multiple artforms and modalities presented alongside sonic musical performance.  From a music+ perspective,  all temporal (and contextual) elements of these artworks can be considered to musical elements able to be composed, arranged, performed, or improvised.

{{< youtube l1nR9ZJaA90 >}} 
{{< youtube QgPrnCmsGY8 >}}


Out of the various modalities, the relationship between vision and sound is likely the most formalised.

* John Whitney  (Visual Music)
* http://www.jaroslawkapuscinski.com/
* https://www.freidaabtan.com/
* Jonathan Sterne's Audio-visual Litany
* Michel Chion's Audio-vision and Visu-audition

Some of my previous work has been attempting to extract the musical qualities present in visual stimuli or vice versa - translating between vision and sound, sound and vision.

Adelaide to Melbourne - expressing audibly changes in vision over time

Luminesce - expressing visually changes in sound over time

But here are other modalities with latent musical potential
 
* Smell
* Touch
* Temperature
* Soma
* **Proprioception**

... Motion is

-> change in position over time (!)

Other intermodal composers often talk about sonic or visual motion (kapuscinski, whitney)

What is super cool about motion is we all have predictive models about the physical world - throw a ball up, we expect it to come down. This predictability gives a substrate for musical experience.

Ball example.

Glider example.

**Aileron - expressing audibly changes in position over time**
  

Examples of physics based prediction (bouncing a ball on a drum)

So.... let's say we want to build a musical system that takes as an input some kind of motion, and as an output produces some sort of sound.  If we take on this task, we are leaving behind the safe confines of deterministic music and entering the hazy abyss that is...

---

  ## DYNAMIC MUSIC

* dynamic music - tracy redhead, florian thalmann
* encapsulates adaptive, generative, stochastic music and more.


### Tools for Dynamic Music composition

  
* Music for games is a major domain of Dynamic Music. Most common tools are FMod, and WWise.
* I wanted more granular control so I developed my own suites of software tools.

  

---

  ## SPATIAL METACOMPOSITION


Compositing for motion in digital virtual space

Distinct from Spatial Music

Create a multidimensional musical space.

Explore that space to render a musical result.

Each performance of the metacompostion may be different, so you are infact composing an infinite set of musical possiblities.

Dimensions of this compositional space do not necessarily conform to those of physical space.

Rotation example

Two dimensions of position

Change one axis to acceleration

or temperature

or character health

  

Now we are creating a generalised, multi-dimensional compositional space.

  

---

### The Hyperstave

  

### Aileron

* Limited by Abletons clipslots.
* investigated tools b composing Aileron One

Primary take away from this was changes in velocity can change our musical perception (unified musical time spectrum). We can manage this by bracketing velocities and composing transitions between these brackets. Akin to metric modulation.

*This applies to the rate of change of any continuous data source*

  
### Elevon 🛩

* Chose Pure Data because it is cross platform and embedable (raspberry pi).
* Hyperstave development became Elevon (structure)
* Music making tools became bop (content)
* Examples of bop - Elevon One

[bop](https://github.com/zealtv/bop)

[elevon](https://github.com/zealtv/elevon)

[Aileron](https://github.com/zealtv/Aileron)

### bop 🐦

{{< youtube GnQuqaytKiI >}}
  
---
## Takeaways 🍱

* Musical perception can be elicited through other senses - although this may not be consciously recognised by the experiencer.
* Musical perception is based on past experience - we can (and do) push at the boundaries of musical perception to build the basis for new musical experience (Coltrane,  the trajectory through musical history of consonance to dissonance, Stravinsky).
* Technology opens a myriad of creative possibilities - we build upon the existing collective musical memory to slowly expand humanities palette of aesthetic experience.
* The medium of Dynamic Music allow us to create musical representations of arbitrary data.  When applied to motion data we are able to create musical perceptions of changes in position over time.
  