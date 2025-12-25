#### eevo literate programming

## Convert eevo files into markdown documentation

## - Regular lines are code blocks
## - Single semicolon `;` are comments inside code blocks
## - Double semicolon `;;` are paragraphs.
## - Triple semicolon `;;;` marks subsection headers.
## - Quadruple semicolon `;;;;` marks section headers.

### TODO: write in eevo
## - eval eevo unquoted (like scribble)

# BEGIN {
# 	print "---"
# 	print "title: " FILENAME
# 	print "---\n"
# }

## Headings
$1 ~ /^;{2,4}$/ {
	if ($1 == ";;;;") # heading
		printf "# "
	else if ($1 == ";;;") # sub-heading
		printf "## "
	for (i = 2; i < NF; i++) # rest of line
		printf $i " "
	print $NF
}

## Code blocks
$1 !~ /;;/ {
	# /[^[:space:]]/
	if (NF > 0) printf "\t"
	print $0
}

## Save all lines to be displayed at the end
{ lines[NR] = $0 }

## Append source file to end in expandable button
## TODO: add option to enable
END {
	print "<details><summary>Source File</summary>"
	print "\n```"
	for (i = 1; i <= NR; i++)
		print lines[i]
	print "```"
	print "</details>"
}
