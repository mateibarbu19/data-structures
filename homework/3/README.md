# Homework 3 - Academia Network

Description: <https://ocw.cs.pub.ro/courses/sd-ca/teme/tema3-2020>.

The homework has been designed as a website so you can easily navigate from a
paper to others that cite it.

To solve this I created a struct for Paper and a struct for Authors.

Then I linked the authors and [aper using pointers from a paper to all it's
authors and from a author to all his/hers papers. Then for all tasks I created a
hashtable for the authors and papers.

We have modified the sources from the laboratories in order to have a better
coherence between the names, using mostly Hashtables.

For task 2, the function links all the papers in the sorted domains in a
Hashtable.

For task 3, we check for each job if the source has already been found in the
system and if we do not find the source in the paper hashtable, we add the paper
to the waiting list created in a waiting Hashtable.

For task 4 (erdos) we chose to link the authors together, because it was a much
easier option, for the authors who worked on the same papers, to avoid building
a graph. I made for each author a compiled list of all the colleagues he worked
with. At the end just do a BFS tour.

For task 7 we used hashtables, and the keys were the institutions linked to the
domain and we only remember the unique authors who meet these criteria.

Besides this, I feel the code is pretty self explanatory + accompanied by many
comments (written in English).

Because of the size that a test database has, the **checker was not provided**.
Check out the OCW link for it.