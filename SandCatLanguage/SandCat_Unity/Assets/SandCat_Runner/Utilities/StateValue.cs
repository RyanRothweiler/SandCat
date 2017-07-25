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
	public int arrayIndex;

	public float GetValue()
	{
		if (type == StateType.Entity) {
			return (SandCat.instance.GetEntityFluentValue(entityName, fluentName));
		} else if (type == StateType.Fluent) {
			return (SandCat.instance.GetFluentValue(fluentName));
		} else if (type == StateType.Array) {
			return (SandCat.instance.GetFluentInArray(entityName, arrayIndex, fluentName));
		}

		return (0);
	}
}
