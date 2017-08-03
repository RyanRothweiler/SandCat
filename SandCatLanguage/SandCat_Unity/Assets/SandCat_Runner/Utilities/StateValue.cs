using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum StateType {
	Entity, Fluent, Array
}

[System.Serializable]
public class StateValue : MonoBehaviour
{

	public StateType type;

	public string fluentName;
	public string entityName;
	public string arrayName;

	public bool isTwoD;
	public int arrayIndex;
	public int arrayX;
	public int arrayY;

	public float GetValue()
	{
		if (type == StateType.Entity) {
			return (SandCat.instance.GetEntityFluentValue(entityName, fluentName));
		} else if (type == StateType.Fluent) {
			return (SandCat.instance.GetFluentValue(fluentName));
		} else if (type == StateType.Array) {
			SandCat.instance.CheckArrayExists(arrayName);

			if (isTwoD) {
				int height = SandCat.instance.GetArrayHeight(arrayName);
				arrayIndex = ((arrayX - 1) * height) + (arrayY - 1) + 1;
			}

			return (SandCat.instance.GetFluentInArray(arrayName, arrayIndex, fluentName, this.name));
		}

		return (0);
	}
}
