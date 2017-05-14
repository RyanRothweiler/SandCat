using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ValuesToSprite : MonoBehaviour
{
	[System.Serializable]
	public class Pair
	{
		public Sprite image;
		public float value;
	}

	public Pair[] pairings;
}