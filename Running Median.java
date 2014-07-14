import java.util.*;

public class Median {

	private class IndexMaxPQ<Key extends Comparable<Key>> implements Iterable<Integer> {
		private int N;           // number of elements on PQ
		private int[] pq;        // binary heap using 1-based indexing
		private int[] qp;        // inverse of pq - qp[pq[i]] = pq[qp[i]] = i
		private Key[] keys;      // keys[i] = priority of i

	   /**
		 * Create an empty indexed priority queue with indices between 0 and NMAX-1.
		 * @throws java.lang.IllegalArgumentException if NMAX < 0
		 */
		public IndexMaxPQ(int NMAX) {
			keys = (Key[]) new Comparable[NMAX + 1];    // make this of length NMAX??
			pq   = new int[NMAX + 1];
			qp   = new int[NMAX + 1];                   // make this of length NMAX??
			for (int i = 0; i <= NMAX; i++) qp[i] = -1;
		}

	   /**
		 * Is the priority queue empty?
		 */
		public boolean isEmpty() { return N == 0; }

	   /**
		 * Is i an index on the priority queue?
		 * @throws java.lang.IndexOutOfBoundsException unless (0 &le; i < NMAX)
		 */
		public boolean contains(int i) {
			return qp[i] != -1;
		}


	   /**
		 * Return the number of keys on the priority queue.
		 */
		public int size() {
			return N;
		}

	   /**
		 * Associate key with index i.
		 * @throws java.lang.IndexOutOfBoundsException unless 0 &le; i < NMAX
		 * @throws java.util.IllegalArgumentException if there already is an item associated with index i.
		 */
		public void insert(int i, Key key) {
			if (contains(i)) throw new IllegalArgumentException("index is already in the priority queue");
			N++;
			qp[i] = N;
			pq[N] = i;
			keys[i] = key;
			swim(N);
		}

	   /**
		 * Return a minimal key.
		 * @throws java.util.NoSuchElementException if priority queue is empty.
		 */
		public Key maxKey() { 
			if (N == 0) throw new NoSuchElementException("Priority queue underflow");
			return keys[pq[1]];
		}

	   /**
		 * Delete a maximal key and return its associated index.
		 * @throws java.util.NoSuchElementException if priority queue is empty.
		 */
		public int delMax() { 
			if (N == 0) throw new NoSuchElementException("Priority queue underflow");
			int min = pq[1];        
			exch(1, N--); 
			sink(1);
			qp[min] = -1;            // delete
			keys[pq[N+1]] = null;    // to help with garbage collection
			pq[N+1] = -1;            // not needed
			return min; 
		}

	   /**
		 * Delete the key associated with index i.
		 * @throws java.lang.IndexOutOfBoundsException unless 0 &le; i < NMAX
		 * @throws java.util.NoSuchElementException no key is associated with index i
		 */
		public void delete(int i) {
			if (!contains(i)) throw new NoSuchElementException("index is not in the priority queue");
			int index = qp[i];
			exch(index, N--);
			swim(index);
			sink(index);
			keys[i] = null;
			qp[i] = -1;
		}


	   /**************************************************************
		* General helper functions
		**************************************************************/
		private boolean less(int i, int j) {
			return keys[pq[i]].compareTo(keys[pq[j]]) < 0;
		}

		private void exch(int i, int j) {
			int swap = pq[i]; pq[i] = pq[j]; pq[j] = swap;
			qp[pq[i]] = i; qp[pq[j]] = j;
		}


	   /**************************************************************
		* Heap helper functions
		**************************************************************/
		private void swim(int k)  {
			while (k > 1 && less(k/2, k)) {
				exch(k, k/2);
				k = k/2;
			}
		}

		private void sink(int k) {
			while (2*k <= N) {
				int j = 2*k;
				if (j < N && less(j, j+1)) j++;
				if (!less(k, j)) break;
				exch(k, j);
				k = j;
			}
		}


	   /***********************************************************************
		* Iterators
		**********************************************************************/

	   /**
		 * Return an iterator that iterates over all of the elements on the
		 * priority queue in descending order.
		 * <p>
		 * The iterator doesn't implement <tt>remove()</tt> since it's optional.
		 */
		public Iterator<Integer> iterator() { return new HeapIterator(); }

		private class HeapIterator implements Iterator<Integer> {
			// create a new pq
			private IndexMaxPQ<Key> copy;

			// add all elements to copy of heap
			// takes linear time since already in heap order so no keys move
			public HeapIterator() {
				copy = new IndexMaxPQ<Key>(pq.length - 1);
				for (int i = 1; i <= N; i++)
					copy.insert(pq[i], keys[pq[i]]);
			}

			public boolean hasNext()  { return !copy.isEmpty();                     }
			public void remove()      { throw new UnsupportedOperationException();  }

			public Integer next() {
				if (!hasNext()) throw new NoSuchElementException();
				return copy.delMax();
			}
		}
	}


	private class IndexMinPQ<Key extends Comparable<Key>> implements Iterable<Integer> {
		private int NMAX;        // maximum number of elements on PQ
		private int N;           // number of elements on PQ
		private int[] pq;        // binary heap using 1-based indexing
		private int[] qp;        // inverse of pq - qp[pq[i]] = pq[qp[i]] = i
		private Key[] keys;      // keys[i] = priority of i

	   /**
		 * Create an empty indexed priority queue with indices between 0 and NMAX-1.
		 * @throws java.lang.IllegalArgumentException if NMAX < 0
		 */
		public IndexMinPQ(int NMAX) {
			if (NMAX < 0) throw new IllegalArgumentException();
			this.NMAX = NMAX;
			keys = (Key[]) new Comparable[NMAX + 1];    // make this of length NMAX??
			pq   = new int[NMAX + 1];
			qp   = new int[NMAX + 1];                   // make this of length NMAX??
			for (int i = 0; i <= NMAX; i++) qp[i] = -1;
		}

	   /**
		 * Is the priority queue empty?
		 */
		public boolean isEmpty() { return N == 0; }

	   /**
		 * Is i an index on the priority queue?
		 * @throws java.lang.IndexOutOfBoundsException unless (0 &le; i < NMAX)
		 */
		public boolean contains(int i) {
			if (i < 0 || i >= NMAX) throw new IndexOutOfBoundsException();
			return qp[i] != -1;
		}

	   /**
		 * Return the number of keys on the priority queue.
		 */
		public int size() {
			return N;
		}

	   /**
		 * Associate key with index i.
		 * @throws java.lang.IndexOutOfBoundsException unless 0 &le; i < NMAX
		 * @throws java.util.IllegalArgumentException if there already is an item associated with index i.
		 */
		public void insert(int i, Key key) {
			if (i < 0 || i >= NMAX) throw new IndexOutOfBoundsException();
			if (contains(i)) throw new IllegalArgumentException("index is already in the priority queue");
			N++;
			qp[i] = N;
			pq[N] = i;
			keys[i] = key;
			swim(N);
		}

	   /**
		 * Return a minimal key.
		 * @throws java.util.NoSuchElementException if priority queue is empty.
		 */
		public Key minKey() { 
			if (N == 0) throw new NoSuchElementException("Priority queue underflow");
			return keys[pq[1]];        
		}

	   /**
		 * Delete a minimal key and return its associated index.
		 * @throws java.util.NoSuchElementException if priority queue is empty.
		 */
		public int delMin() { 
			if (N == 0) throw new NoSuchElementException("Priority queue underflow");
			int min = pq[1];        
			exch(1, N--); 
			sink(1);
			qp[min] = -1;            // delete
			keys[pq[N+1]] = null;    // to help with garbage collection
			pq[N+1] = -1;            // not needed
			return min; 
		}
		
	   /**
		 * Delete the key associated with index i.
		 * @throws java.lang.IndexOutOfBoundsException unless 0 &le; i < NMAX
		 * @throws java.util.NoSuchElementException no key is associated with index i
		 */
		public void delete(int i) {
			if (i < 0 || i >= NMAX) throw new IndexOutOfBoundsException();
			if (!contains(i)) throw new NoSuchElementException("index is not in the priority queue");
			int index = qp[i];
			exch(index, N--);
			swim(index);
			sink(index);
			keys[i] = null;
			qp[i] = -1;
		}


	   /**************************************************************
		* General helper functions
		**************************************************************/
		private boolean greater(int i, int j) {
			return keys[pq[i]].compareTo(keys[pq[j]]) > 0;
		}

		private void exch(int i, int j) {
			int swap = pq[i]; pq[i] = pq[j]; pq[j] = swap;
			qp[pq[i]] = i; qp[pq[j]] = j;
		}


	   /**************************************************************
		* Heap helper functions
		**************************************************************/
		private void swim(int k)  {
			while (k > 1 && greater(k/2, k)) {
				exch(k, k/2);
				k = k/2;
			}
		}

		private void sink(int k) {
			while (2*k <= N) {
				int j = 2*k;
				if (j < N && greater(j, j+1)) j++;
				if (!greater(k, j)) break;
				exch(k, j);
				k = j;
			}
		}


	   /***********************************************************************
		* Iterators
		**********************************************************************/

	   /**
		 * Return an iterator that iterates over all of the elements on the
		 * priority queue in ascending order.
		 * <p>
		 * The iterator doesn't implement <tt>remove()</tt> since it's optional.
		 */
		public Iterator<Integer> iterator() { return new HeapIterator(); }

		private class HeapIterator implements Iterator<Integer> {
			// create a new pq
			private IndexMinPQ<Key> copy;

			// add all elements to copy of heap
			// takes linear time since already in heap order so no keys move
			public HeapIterator() {
				copy = new IndexMinPQ<Key>(pq.length - 1);
				for (int i = 1; i <= N; i++)
					copy.insert(pq[i], keys[pq[i]]);
			}

			public boolean hasNext()  { return !copy.isEmpty();                     }
			public void remove()      { throw new UnsupportedOperationException();  }

			public Integer next() {
				if (!hasNext()) throw new NoSuchElementException();
				return copy.delMin();
			}
		}
	}
	
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
	/*
		try {
			in = new Scanner(new File("C:\\Users\\kartik\\Downloads\\input01.txt"));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	 */     
	    int N = in.nextInt(), iMin = 0, iMax = 0;
	    Boolean exchange = false;
	    Median inst = new Median();
	    MinPQ = inst.new IndexMinPQ<Long>(5*N + 1);
	    MaxPQ = inst.new IndexMaxPQ<Long>(5*N + 1);
	    
	    for(int i=0; i<N; i++)	{
	    	String op = in.next();
	    	long x = in.nextLong();
	    	
	    // handle the remove operation
	    	if(op.equals("r"))	{
	    		if(indexMin.containsKey(x))	{
	    			int index = indexMin.get(x).first();
	    			removeFromMap(x, index, indexMin);
	    			MinPQ.delete(index);
	    			exchange = false;
	    			if(MinPQ.size() < MaxPQ.size())	{	// delete from MaxPQ and insert into MinPQ
	    				exchange = true;
	    				long key1 = MaxPQ.maxKey();
	    				int index1 = MaxPQ.delMax();
	    				removeFromMap(key1, index1, indexMax);
	    				
	    				MinPQ.insert(iMin, key1);
						addToMap(key1, iMin, indexMin);
						iMin++;
	    			}
	    		} else if(indexMax.containsKey(x))	{
	    			int index = indexMax.get(x).first();
	    			removeFromMap(x, index, indexMax);
	    			MaxPQ.delete(index);
	    			exchange = true;
	    			if(MinPQ.size() > MaxPQ.size() + 1)	{	// delete from MinPQ and insert into MaxPQ
	    				exchange = false;
	    				long key1 = MinPQ.minKey();
	    				int index1 = MinPQ.delMin();
	    				removeFromMap(key1, index1, indexMin);
	    				
	    				MaxPQ.insert(iMax, key1);
						addToMap(key1, iMax, indexMax);
						iMax++;
	    			}
	    		} else	{
	    			System.out.println("Wrong!");
	    			continue;
	    		}
	    	} 
	    // handle the add operation
	    	else if(op.equals("a"))	{
	    		MinPQ.insert(iMin, x);
	    		addToMap(x, iMin, indexMin);
	    		iMin++;
	    		if(exchange)	{
	    			long key = MinPQ.minKey();
	    			int index = MinPQ.delMin();
	    			removeFromMap(key, index, indexMin);
	    			MaxPQ.insert(iMax, key);
	    			addToMap(key, iMax, indexMax);
	    			iMax++;
	    			exchange = false;
	    		} else	{
	    			exchange = true;
	    			if(!MaxPQ.isEmpty() && MaxPQ.maxKey() > MinPQ.minKey())	{
	    				long key1 = MaxPQ.maxKey();
	    				int index1 = MaxPQ.delMax();
	    				removeFromMap(key1, index1, indexMax);

		    			long key = MinPQ.minKey();
	    				int index = MinPQ.delMin();
		    			removeFromMap(key, index, indexMin);
						MaxPQ.insert(iMax, key);
		    			addToMap(key, iMax, indexMax);
		    			iMax++;
		    			
						MinPQ.insert(iMin, key1);
						addToMap(key1, iMin, indexMin);
						iMin++;
	    			}
	    		}
	    	}

	 	    if(MinPQ.size() > MaxPQ.size())	{
		    	long result = MinPQ.minKey();
			    System.out.println( result );
		    } else if(!MaxPQ.isEmpty())	{
		    	long result = MinPQ.minKey() + MaxPQ.maxKey();
		    	if(result % 2 == 0)	{
				    System.out.println( result / 2 );
		    	} else	{
		    		if(result == -1)
		    			System.out.println("-0.5");
		    		else
		    			System.out.println( String.valueOf(result / 2) + ".5" );
		    	}		    	
		    } else	{
    			System.out.println("Wrong!");
		    }
	    }

	}
	
	static void addToMap(long key, int val, TreeMap<Long, TreeSet<Integer>> map)	{
		if(map.containsKey(key))
			map.get(key).add(val);
		else	{
			TreeSet<Integer> arg = new TreeSet<Integer>();
			arg.add(val);
			map.put(key, arg);
		}
	}
	
	static void removeFromMap(long key, int val, TreeMap<Long, TreeSet<Integer>> map)	{
		TreeSet<Integer> arg = map.get(key);
		arg.remove(val);
		if(arg.isEmpty())
			map.remove(key);
	}
	
	private static TreeMap<Long, TreeSet<Integer>> indexMin = new TreeMap<Long, TreeSet<Integer>>();
	private static TreeMap<Long, TreeSet<Integer>> indexMax = new TreeMap<Long, TreeSet<Integer>>();
	private static IndexMinPQ<Long> MinPQ = null;
	private static IndexMaxPQ<Long> MaxPQ = null;
}