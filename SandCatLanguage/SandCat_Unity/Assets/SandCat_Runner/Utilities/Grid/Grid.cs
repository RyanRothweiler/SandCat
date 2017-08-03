using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grid : MonoBehaviour
{

	public bool showGrid = true;
	public string widthFluentName;
	public string heightFluentName;
	public GameObject gridCell;

	public float gridSize = 2.0f;

	private float prevSize;

	public void Update()
	{
		if (prevSize != gridSize) {
			prevSize = gridSize;

			// Remove all current children
			{
				for (int index = 0; index < this.transform.childCount; index++) {
					Destroy(this.transform.GetChild(index).gameObject);
				}
			}

			// Create new grid
			if (showGrid) {
				int width = (int)SandCat.instance.GetFluentValue(widthFluentName);
				int height = (int)SandCat.instance.GetFluentValue(heightFluentName);

				for (int x = 1; x <= width; x++) {
					for (int y = 1; y <= height; y++) {
						GameObject cell = Instantiate(gridCell);
						cell.transform.SetParent(this.transform, false);
						cell.transform.position = GridToWorldPos(new Vector2(x, y));

						GridCell gc = cell.AddComponent<GridCell>();
						// NOTE the +1 is because SandCat arrays start at 0.
						gc.x = x;
						gc.y = y;
					}
				}
			}

		}
	}

	public Vector3 GridToWorldPos(Vector2 gridPos)
	{
		return (this.transform.position + new Vector3((gridPos.x - 1) * gridSize, (gridPos.y - 1) * gridSize, 0.0f));
	}
}
