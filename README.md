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
./build/Linux-x86_64/bin/bowstring <command> [options]

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

To convert between two graph formats, use the `convert` action.

```
./bowstring convert
```
