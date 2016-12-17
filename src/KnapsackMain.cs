using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Knapsack
{
    class Program
    {
        static void Main(string[] args)
        {
            TestCase tc = new TestCase();
            //tc.TestCase1();
            //tc.TestCase0();
            tc.TestCase2();
        }
    }

    class TestCase
    {
        /// <summary>
        /// Test 0-1 knapsack problem with random values and weights.
        /// </summary>
        public void TestCase0()
        {
            const int TOTAL_ITEMS_NUM = 20;
            const double WEIGHT_MEAN = 4.0;
            const double VALUE_MEAN = 10.0;
            const double CAPACITY = 40.0;

            var v = KnapsackSol.GenerateGroceries(TOTAL_ITEMS_NUM, WEIGHT_MEAN, VALUE_MEAN, 1);
            WeightNode wn = KnapsackSol.Process(v, CAPACITY);
            Console.WriteLine(wn.ToString());
        }
        /// <summary>
        /// Test 0-1 knapsack problem with fixed values and weights.
        /// </summary>
        public void TestCase1()
        {
            const int ITEM_NUM = 10;
            const double KNAPSACK_CAPACITY = 30;
            double[] weight = new double[ITEM_NUM] { 1, 9, 3, 6, 5, 4, 7, 2, 8, 10 };
            double[] price = new double[ITEM_NUM] { 2, 8, 9, 4, 7, 3, 1, 6, 5, 10 };
            List<Item> l = KnapsackSol.GenerateItems(weight, price);
            WeightNode wn = KnapsackSol.Process(l, KNAPSACK_CAPACITY); 
            Console.Write( wn.ToString() );
        }
        /// <summary>
        /// Test closest more than value threshold problem.
        /// </summary>
        public void TestCase2()
        {
            const double KNAPSACK_CAPACITY = 300;

            const int ITEM_NUM = 16;
            double[] weight = new double[ITEM_NUM] { 355, 367, 192, 100, 228, 456, 2714, 157, 151.9, 53, 98, 210, 111, 266, 276, 94 };
            double[] price = new double[ITEM_NUM] { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

            int i = 0;
            foreach (double w in weight)
            {
                if ((i++ % 4) == 0)
                {
                    Console.Write("\n");
                }
                Console.Write("  {0,7},", w.ToString("F2"));
            }
            List<Item> l = KnapsackSol.GenerateItems(weight, price);

            do
            {
                Console.WriteLine("\nPlease input a number as capacity ('Q' to quit):");
                string str = Console.ReadLine();

                double capacity;
                if (false == double.TryParse(str, out capacity))
                {
                    if (str.CompareTo("Q") == 0)
                    {
                        Console.WriteLine("You quit the program!");
                        break;
                    }
                    else
                    {
                        Console.WriteLine("Can't recognize the input number: {0}", str);
                        continue;
                    }
                }
                else
                {
                    WeightNode wn = KnapsackSol.FindItemCollectionWithMostMatchValue(l, capacity);
                    if (wn != null)
                    {
                        Console.WriteLine(wn.ToString());
                    }
                    else
                    {
                        Console.WriteLine("No solution!");
                    }
                }
            } while (true);
        }


    }
}
