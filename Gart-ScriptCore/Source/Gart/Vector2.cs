namespace Gart
{
    public struct Vector2
    {
        public float x, y;

        public static Vector2 Zero => new Vector2(0.0f);
        public static Vector2 Up => new Vector2(0.0f,1.0f);

        public Vector2(float scalar)
        {  
            x = scalar; 
            y = scalar;
        }
        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.x + b.x, a.y + b.y);
        }

        public static Vector2 operator *(Vector2 a, float scalar)
        {
            return new Vector2(a.x * scalar, a.y * scalar);
        }

        public float LenghtSquared()
        {
            return x * x + y * y;
        }

        public float Lenght()
        {
            return (float)System.Math.Sqrt(LenghtSquared());
        }

        
    }
}
