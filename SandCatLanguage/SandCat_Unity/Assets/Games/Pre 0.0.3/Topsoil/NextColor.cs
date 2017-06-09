using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class NextColor : MonoBehaviour
{

	public Fluent fluent;
	public ValuesToColor pairing;

	private Image image;

	// Use this for initialization
	void Start ()
	{
		image = this.GetComponent<Image>();
	}

	// Update is called once per frame
	void Update ()
	{
		int fluentValue = (int)fluent.GetValue() + 1;
		if (fluentValue == 4) {
			fluentValue = 1;
		}

		foreach (ValuesToColor.Pair pair in pairing.pairings) {
			if (fluentValue == pair.value) {
				image.color = pair.color;
				break;
			}
		}
	}
}
