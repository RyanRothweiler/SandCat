using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AWater : MonoBehaviour
{

	public Grid grid;
	public int arrayIndex;

	// Update is called once per frame
	void Update ()
	{
		// place player on grid
		{
			int x = (int)SandCat.instance.GetFluentInArray("Water", arrayIndex, "X");
			int y = (int)SandCat.instance.GetFluentInArray("Water", arrayIndex, "Y");
			this.transform.position = grid.GridToWorldPos(new Vector2(x, y));
		}
	}
}
