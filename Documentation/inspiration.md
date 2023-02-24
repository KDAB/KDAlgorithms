Just for Inspiration
====================
This page contains a number of (well one for now) examples, where you may use algorithms to write clearer code.

It took me (Jesper) quite a long time before I could really appreciate usage of algorithms, and furthermore
I was often struggling to find the right one to use. 
I was also often paralyzed when I had to chose between two solutions to the same problem: 
one using a relative simple for loop and one which was more complex but using algorithms.

Today, I have settled with the understanding that algorithms is a means to an end, not the end itself. 
Their primary purpose is to make the code more expressive - so instead of having to read through a for loop to understand exactly what it is building up, I can see that it for example filters out some items, and transform the result to another format.


Traversing a list which do not provide iterators
------------------------------------------------
(Example in Inspiration/qlistview_all_columns_selected.cpp)

Sometimes you want to filter, transform, or apply any other algorithm to a *list* provided to you via an API which do *not* offer iterators, and which you therefore *can not* apply algorithms on.

One solution is of course to create a wrapper object providing an iterator API, but as this solution shows, another solution is to simply create a list of indexes to iterate.

### The Problem
Imagine you want to check if all columns of a given row in a table (here QTableWidget) is selected. This looks a lot like <a href="https://github.com/KDAB/KDAlgorithms/blob/main/Documentation/algorithms.md#all_any_none">all_of</a>, doesn't it?

To get to the selected items, you will have to ask the table for its QItemSelectionModel, which in turn, naturally does not offer a *are all columns for a given row selected* API, nor does it offer an iterator to traverse all columns for a given row (which would have been a really weird API to begin with, if it did).

What the selection model does offer you is to answer yes or no to the question is a given (row, column) selected.

A solution often looks like this:

```
    bool allColumnsSelected = true;
    int row = m_table->currentRow();
    auto selectionModel = m_table->selectionModel();
    auto model = m_table->model();
    for (int i = 0; i < m_table->columnCount(); ++i) {
        if (!selectionModel->isSelected(model->index(row, i))) {
            allColumnsSelected = false;
            break;
        }
    }
    
    if (allColumnsSelected)
        doSomething();
```

The code is proper Qt code, and might not be super readable to you if you are not a Qt developer, but even if you are, you need to read all lines carefully to understand it. The key part in the above is the explicit for loop over all columns.

Here is a solution using KDAlgorithms, and more specifically **all_of**:

```
    auto columns = kdalgorithms::iota(m_table->columnCount());
    auto isColumnSelected = [row = m_table->currentRow(),
                             selectionModel = m_table->selectionModel(),
                             model = m_table->model()](int column) {
        return selectionModel->isSelected(model->index(row, column));
    };

    if (kdalgorithms::all_of(columns, isColumnSelected))
        doSomething();
```

The trick is to iterate (implicitly inside the algorithms) over the list of indexes for the columns. We do so by first fetching a list of all indexes (the variable *columns*). Following that we create a lambda expression to answer if a given column is selected, and now the test is super readable.

### Some after thoughts
If you have that feeling that you have seen this trick before, namely creating a list of the indexes and operating on that, then chances are you occasionally write Python code. In python you often find code like:

```
vector = [1, 2, 3, 4, 5]
for i in range(len(vector)):
    if vector[i] % 2 == 1:
        vector[i] = 0
```        

