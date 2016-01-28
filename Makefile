
.PHONY: all clean

all: lesson.html

lesson.html: lesson.md
	pandoc -f markdown_github -t html --toc -s -c swc.css $< -o $@

clean:
	rm lesson.html
