using System;

namespace Game.Math
{
    [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
    public struct Vector2 : IEquatable<Vector2>
    {
        public float X;
        public float Y;

        public static Vector2 Zero = new Vector2(0, 0);
        public static Vector2 UnitX = new Vector2(1, 0);
        public static Vector2 UnitY = new Vector2(0, 1);

        public Vector2(float x, float y)
        {
            this.X = x;
            this.Y = y;
        }

        public void Normalize()
        {
            float len = (float)(1.0 / System.Math.Sqrt(X * X + Y * Y));
            X *= len;
            Y *= len;
        }

        public static Vector2 Normalize(Vector2 v)
        {
            Vector2 result = v;
            result.Normalize();
            return result;
        }

        public Vector2 GetNormalized()
        {
            float len = (float)(System.Math.Sqrt(X * X + Y * Y));
            if (len == 0)
            {
                return Zero;
            }
            else
            {
                return new Vector2(X / len, Y / len);
            }
        }

        public float Length
        {
            get
            {
                return (float)System.Math.Sqrt(X * X + Y * Y);
            }
        }

        public float Length2
        {
            get
            {
                return X * X + Y * Y;
            }
        }

        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            return new Vector2(a.X + (b.X - a.X) * t,
                               a.Y + (b.Y - a.Y) * t);
        }

        public static void Lerp(ref Vector2 a, ref Vector2 b, float t, out Vector2 c)
        {
            c.X = a.X + (b.X - a.X) * t;
            c.Y = a.Y + (b.Y - a.Y) * t;
        }

        public static float Dot(Vector2 a, Vector2 b)
        {
            return a.X * b.X + a.Y * b.Y;
        }

        public override string ToString()
        {
            return String.Format("{0}, {1}", X, Y);
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ Y.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            if (obj is Vector2)
            {
                return Equals((Vector2)obj);
            }
            return false;
        }

        public bool Equals(Vector2 other)
        {
            return X == other.X && Y == other.Y;
        }

        public static bool operator ==(Vector2 a, Vector2 b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(Vector2 a, Vector2 b)
        {
            return !a.Equals(b);
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        public static Vector2 operator -(Vector2 a)
        {
            return new Vector2(-a.X, -a.Y);
        }

        public static Vector2 operator *(Vector2 a, float b)
        {
            return new Vector2(a.X * b, a.Y * b);
        }

        public static Vector2 operator *(float a, Vector2 b)
        {
            return new Vector2(a * b.X, a * b.Y);
        }

        public static Vector2 operator /(Vector2 a, float b)
        {
            return new Vector2(a.X / b, a.Y / b);
        }
    }
}
