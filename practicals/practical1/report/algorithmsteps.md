Algorithm:

Part 1: Connect all nodes/subclusters
If not one subcluster:
  Step 1: Select the subcluster with the longest path. (save this)
  Step 2: Select a middle node of the longest path and save this in variable middleNode (it stays the same).
While it is not one subcluster:
  Step 3: Select a middle node of any other subcluster randomMiddleNode.
  Step 4: Connect randomMiddleNode to middleNode.
Do this until we get only one subcluster.

Part 2:
  Step 1: Find the longest path in the remaining cluster and subtract the end nodes.

Return the length of the found path.
