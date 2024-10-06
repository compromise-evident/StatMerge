Run it: ```apt install g++ geany```. Open the .cpp in Geany. Hit F9 once. F5 to run.

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/StatMerge/main/Other/Terminal_2a2c24eae143b3f4a2bfe468fb732cba.png">
</p>

<br>
<br>

## Top-occurring Bytes are final, else the smallest top-occurring Bytes

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/StatMerge/main/Other/What_it_does_b53a4b1452da928659635b21abd83f30418c241ee2af94802f0f30e151a99787.png">
</p>

<br>
<br>

## Merge PyTorch models (.pth)

That's right, you can merge PyTorch models stored as efficient full ASCII.
Preferably, merge based on byte occurrence (default in StatMerge.cpp.)
And preferably, merge models that are copies of one but trained in any way,
or merge models that are snapshots of a model being updated forever.
(This is fine because model training is nondeterministic by default.)

See, merging models that were initialized randomly then trained,
leads to a model that appears random--exactly as useless as
one initialized randomly and never trained.
Instead, it appears effective to merge
models whose many parameters
are equal or close.
Merge at least 50+ models at once.
In my experience, such a merge results in a model unique to the 50,
and whose usefulness is representative of the best-performing of those 50.

<br>
<br>

## And for Byte average-based merging

Set ```average_instead_of_occurrence``` to true.
Thanks to the way averaging works, if each file begins with "model_23"
for example, that header is carried over to the resulting file. Same for occurrence.
