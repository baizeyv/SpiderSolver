
namespace SpiderRandom
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.Write("-1");
                return;
            }

            if (int.TryParse(args[0], out var seed) && int.TryParse(args[1], out var count))
            {
                var list = SharpRandom.GetRandomNumbers(seed, count);
                Console.Write(string.Join(',', list));
                return;
            }
        
            Console.Write("-1");
        }
    }
}