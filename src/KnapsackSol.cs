using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Knapsack
{
    public class Item
    {
        public double Weight = 1.0;
        public double Value = 1.0;
        public double Number = 1.0;
    }
    public class WeightNode
    {
        public double _Value;
        public double _Weight;
        public List<Item> ItemsList = new List<Item>();
        public WeightNode()
        {
        }
        public override string ToString()
        {
            
            string[] strArray = new string[ ItemsList.Count() ];
            int i = 0;
            foreach (Item g in ItemsList)
            {
                strArray[i++] = string.Format("{0}({1})", g.Value.ToString("F1"), g.Weight.ToString("F1"));
            }
            return string.Format("Total Value(Weight): {0}({1})\n{2}", _Value.ToString("F1"), _Weight.ToString("F1"), string.Join(" ", strArray));
        }
        public WeightNode( WeightNode node )
        {
            _Value = node._Value;
            _Weight = node._Weight;
            ItemsList.AddRange( node.ItemsList );
        }
        public void AddItem( Item g )
        {
            ItemsList.Add( g );
            _Value += g.Value;
            _Weight += g.Weight;
        }
    }
    public class KnapsackSol
    {
        /// <summary>
        /// 
        /// 
        /// </summary>
        /// <param name="TotalNum"></param>
        /// <param name="WeightMean">The mean of all groceries' weight</param>
        /// <param name="ValueMean">The mean of all groceries' value</param>
        /// <param name="MaxNumOfUnit">the  maximum Number of each grocery</param>
        /// <returns></returns>
        public static List<Item> GenerateGroceries( int TotalNum, double WeightMean, double ValueMean, int MaxNumOfUnit)
        {
            int i;
            Random ran = new Random(1 );
            if ( ( TotalNum == 0 ) || ( MaxNumOfUnit < 1 ) )
            {
                return null;
            }
            List<Item> listOfItems = new List<Item>();
            for (i = 0; i < TotalNum; i++)
            {
                Item g = new Item()
                {
                    Number = (MaxNumOfUnit == 1) ? 1 : ran.Next(1, MaxNumOfUnit),
                    Weight = ran.NextDouble() * 2.0 * WeightMean,
                    Value = ran.NextDouble() * 2.0 * ValueMean
                };
                listOfItems.Add(g);
            }
            return listOfItems;
        }
        public static List<Item> GenerateItems(IEnumerable<double> Weights, IEnumerable<double>Values)
        {
            if ( ( Weights.Count() == 0 ) || ( Values.Count() == 0 ) )
            {
                return null;
            }
            List<Item> listOfItems = new List<Item>();
            IEnumerator<double> itr = Values.GetEnumerator();
            foreach (double item in Weights)
            {
                if (false == itr.MoveNext())
                {
                    break;
                }
                Item g = new Item() { Number = 1, Value = itr.Current, Weight = item };
                listOfItems.Add(g);
                
            }
            return listOfItems;
        }
        /// <summary>
        /// Select some grocery from Groceries and make sure
        /// 1. the total weight is less than MaxWeight
        /// 2. the total value is highest.
        /// 
        /// </summary>
        /// <param name="Groceries"></param>
        /// <param name="MaxWeight"></param>
        /// <returns></returns>
        public static WeightNode Process(IEnumerable<Item> Groceries, double MaxWeight)
        {
            /*
             * (Value/Weight)
             * 
             * retList
             * |
             * +- dummy (0.0/0.0)
             * |
             * +- WeightNode (V/W) --> Item0 -> Item1 -> ... -> ItemN
             * |
             * + ...
             * |
             * +- WeightNode (V/Capacity) -> Item0 -> Item1 -> ... ItemN
             * 
             */
            int i;
            WeightNode dummy = new WeightNode() { _Value = 0.0, _Weight = 0.0 };
            LinkedList<WeightNode> retList = new LinkedList<WeightNode>();
            LinkedListNode<WeightNode> node = null;
            LinkedListNode<WeightNode> moreWeighNode = null;
            retList.AddLast(dummy);
            foreach (Item g in Groceries)
            {
                for (i = 0; i < g.Number; i++)
                {
                    node = retList.Last;
                    moreWeighNode = retList.Last;
                    while (node != null)
                    {
                        double totalWeight = node.Value._Weight + g.Weight;
                        if (totalWeight <= MaxWeight)
                        {
                            while( moreWeighNode != null )
                            {
                                if (Math.Abs(totalWeight - moreWeighNode.Value._Weight) < 0.001)
                                {
                                    if (node.Value._Value + g.Value > moreWeighNode.Value._Value)
                                    {
                                        // employ the current node + the current grocery
                                        WeightNode wn = new WeightNode(node.Value);
                                        wn.AddItem(g);
                                        moreWeighNode.Value = wn;
                                    }
                                    break;
                                }
                                else if (totalWeight < moreWeighNode.Value._Weight)
                                {
                                    moreWeighNode = moreWeighNode.Previous;
                                }
                                else if (totalWeight > moreWeighNode.Value._Weight)
                                {
                                    WeightNode wn = new WeightNode(node.Value);
                                    wn.AddItem(g);
                                    //moreWeighNode.AddGrocery(g);
                                    retList.AddAfter(moreWeighNode, new LinkedListNode<WeightNode>(wn));
                                    moreWeighNode = moreWeighNode.Next;
                                    break;
                                }
                            }
                        }
                        node = node.Previous;
                    }
                    
                }
            }
            double maxValue = retList.Max( o => o._Value);
            return retList.Last(o => (o._Value == maxValue));
        }
        /// <summary>
        /// Find the closest collection more than ValueThreshold.
        /// </summary>
        /// <param name="Items"></param>
        /// <param name="WeightThreshold"></param>
        /// <returns></returns>
        public static WeightNode FindItemCollectionWithMostMatchValue(IEnumerable<Item> Items, double WeightThreshold)
        {
            int i;
            int numOfMoreThanThreshold = 0;
            const int BEST_SOLUTION_NUM = 3;
            double maxWeight = 0.0;
            WeightNode dummy = new WeightNode() { _Value = 0.0, _Weight = 0.0 };
            LinkedList<WeightNode> retList = new LinkedList<WeightNode>();
            LinkedListNode<WeightNode> node = null;
            LinkedListNode<WeightNode> moreWeighNode = null;
            retList.AddLast(dummy);
            foreach (Item g in Items)
            {
                for (i = 0; i < g.Number; i++)
                {

                    node = retList.Last;
                    moreWeighNode = retList.Last;
                    maxWeight = retList.Last.Value._Weight;
                    while (node != null)
                    {
                        double totalWeight = node.Value._Weight + g.Weight;
                        while (moreWeighNode != null)
                        {
                            if (Math.Abs(totalWeight - moreWeighNode.Value._Weight) < 0.001)
                            {
                                if (node.Value._Value + g.Value > moreWeighNode.Value._Value)
                                {
                                    // employ the current node + the current grocery
                                    WeightNode wn = new WeightNode(node.Value);
                                    wn.AddItem(g);
                                    moreWeighNode.Value = wn;
                                }
                                break;
                            }
                            else if (totalWeight < moreWeighNode.Value._Weight)
                            {
                                moreWeighNode = moreWeighNode.Previous;
                            }
                            else if (totalWeight > moreWeighNode.Value._Weight)
                            {
                                if ((totalWeight < maxWeight) || (totalWeight < WeightThreshold)
                                    || ((totalWeight > WeightThreshold) && (numOfMoreThanThreshold < BEST_SOLUTION_NUM)))
                                {
                                    WeightNode wn = new WeightNode(node.Value);
                                    wn.AddItem(g);
                                    retList.AddAfter(moreWeighNode, new LinkedListNode<WeightNode>(wn));
                                    moreWeighNode = moreWeighNode.Next;
                                    if (totalWeight > WeightThreshold)
                                    {
                                        if (numOfMoreThanThreshold > BEST_SOLUTION_NUM)
                                        {
                                            retList.RemoveLast();
                                        }
                                        else
                                        {
                                            numOfMoreThanThreshold++;
                                        }
                                    }
                                    maxWeight = retList.Last.Value._Weight;
                                }
                                break;
                            }
                        }
                        node = node.Previous;

                    }

                }
            }
            return retList.First(o => (o._Weight >= WeightThreshold));
        }
        public static List<Item>[] Process(List<Item> Groceries, int MinWeight, int BestSolNum)
        {
            return null;
        }

    }
}
