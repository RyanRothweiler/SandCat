using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crystal : MonoBehaviour
{

	public Grid board;
	public int arrayIndex;

	// Use this for initialization
	void Start ()
	{

	}

	// Update is called once per frame
	void Update ()
	{
		// place on board
		{
			int x = (int)SandCat.instance.GetFluentInArray("Crystals", arrayIndex, "X");
			int y = (int)SandCat.instance.GetFluentInArray("Crystals", arrayIndex, "Y");
			this.transform.position = board.GridToWorldPos(new Vector2(x, y));
		}
	}
}
