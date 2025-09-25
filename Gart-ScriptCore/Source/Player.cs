using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Gart;

namespace Sandbox
{
    public class Player: Entity
    {

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreated - {ID}");
        }

        void OnUpdate(float ts)
        {
            float speed = 1.0f;

            Vector3 translation = Translation;
            Console.WriteLine($"Player.OnUpdate: {ts}, {translation.x}");
            translation.x -= speed * ts;
            Translation = translation;
        }
    }
}
