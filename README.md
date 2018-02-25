Bowstring
=========

A graph processing library.



Building
--------

```
./configure
make
```

Executing
---------

First operation should be to read the help page:

```
./bowstring -h
```

```
USAGE:
./bowstring <command> [options]

Commands:
	help : Display list of available commands
	convert : Convert a graph from one format to another
	order : Order a graph by a given method.
	analyze : Analyze a graph and/or partitions of a graph
	sparsen : Remove edges from a graph
	coordinates : Embbed the vertices in a coordinate space
	flow : Generate a flow on a graph given a source and destination vertex.
	generate : Generate a synthetic graph
```

Individual help pages can be view using the command name and the `-h` flag.

```
./bowstring convert -h
```

To convert between two graph formats, use the `convert` action.

```
./bowstring convert -i some_graph.snap -o some_graph.chaco
```

The format will be determined by the extension. If you are using non-standard
extensions, the format can be specified with the `-I` and `-O` flags.

```
./bowstring convert -i some_graph.txt -Isnap -o some_graph.chaco
```
