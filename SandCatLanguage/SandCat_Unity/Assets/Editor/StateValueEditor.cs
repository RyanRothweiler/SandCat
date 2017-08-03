using UnityEngine;
using UnityEditor;
[CustomEditor(typeof(StateValue))]
public class StateValueEditor : Editor
{

	public override void OnInspectorGUI()
	{
		StateValue stateVal = (StateValue)target;

		stateVal.type = (StateType)EditorGUILayout.EnumPopup("Type", stateVal.type);
		if (stateVal.type == StateType.Entity) {
			stateVal.entityName = EditorGUILayout.TextField("Entity Name", stateVal.entityName);
		}
		if (stateVal.type == StateType.Array) {
			stateVal.arrayName = EditorGUILayout.TextField("Array Name", stateVal.arrayName);
			stateVal.isTwoD = EditorGUILayout.Toggle("Array is 2D", stateVal.isTwoD);
			if (stateVal.isTwoD) {
				stateVal.arrayX = EditorGUILayout.IntField("X", stateVal.arrayX);
				stateVal.arrayY = EditorGUILayout.IntField("Y", stateVal.arrayY);
			} else {
				stateVal.arrayIndex = EditorGUILayout.IntField("Array Index", stateVal.arrayIndex);
			}
		}

		// For all the options we always want fluent name
		stateVal.fluentName = EditorGUILayout.TextField("Fluent Name", stateVal.fluentName);

		EditorUtility.SetDirty(stateVal);
		EditorUtility.SetDirty(stateVal.gameObject);
	}
}