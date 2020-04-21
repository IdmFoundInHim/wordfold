# WordFold [Impromptu Subproject]

## Method (v0.1)

WordFold is a non-commutative binary operation. The domain
for both parameters is all well-ordered multisets. Because the operation
is language-oriented, this program only processes UTF-8 strings. The
parameters are, in order, the base and the glyph. The notation is
`Base .<* Glyph`, read as "base point glyph." The description is:

For every element in base appearing unassociated in the glyph, replace
that base element with the corresponding glyph element and all
subsequent glyph elements until the first associable glyph element.

The algorithm is:

1. Search base for first character of the glyph
1. All character before and including the end of the search are added to
   the result
1. Search glyph (except first character) for a character that is in the
   latter, unsearched portion of base
1. All character before the end of the search are added to the result
1. Repeat with the latter, unsearched portion of base as the base and
   the latter portion of glyph (starting with the character that ended
   the second search) as the glyph, adding on to the end of the result
   until the parameters are exausted
