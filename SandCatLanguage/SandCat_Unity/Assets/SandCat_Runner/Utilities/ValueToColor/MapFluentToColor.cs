using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MapFluentToColor : MonoBehaviour
{

	public Fluent fluent;
	public ValuesToColor pairing;

	private Image image;

	public void Start()
	{
		image = this.GetComponent<Image>();
	}

	// Update is called once per frame
	void Update ()
	{
		int fluentValue = (int)fluent.GetValue();
		foreach (ValuesToColor.Pair pair in pairing.pairings) {
			if (fluentValue == pair.value) {
				image.color = pair.color;
				break;
			}
		}
	}
}
