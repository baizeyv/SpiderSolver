namespace SpiderRandom
{
    public class SharpRandom
    {
        public static List<int> GetRandomNumbers(int seed, int count)
        {
            var random = new Random(seed);
            var result = new List<int>();
            for (var i = 0; i < count; i++)
            {
                result.Add(random.Next());
            }

            return result;
        }
    }
}