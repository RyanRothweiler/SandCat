using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ValuesToColor : MonoBehaviour
{
	[System.Serializable]
	public class Pair
	{
		public Color color;
		public float value;
	}

	public Pair[] pairings;
}