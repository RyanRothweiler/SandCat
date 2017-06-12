using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grid : MonoBehaviour
{

	public string widthFluentName;
	public string heightFluentName;
	public GameObject gridCell;
	public float gridSize = 2.0f;

	void Start ()
	{
		int width = (int)SandCat.instance.GetFluentValue(widthFluentName);
		int height = (int)SandCat.instance.GetFluentValue(heightFluentName);

		for (int x = 0; x < height; x++) {
			for (int y = 0; y < width; y++) {
				GameObject cell = Instantiate(gridCell);
				cell.transform.SetParent(this.transform, false);
				cell.transform.position = GridToWorldPos(new Vector2(x, y));
			}
		}
	}

	public Vector3 GridToWorldPos(Vector2 gridPos)
	{
		return (this.transform.position + new Vector3(gridPos.x * gridSize, gridPos.y * gridSize, 0.0f));
	}
}
