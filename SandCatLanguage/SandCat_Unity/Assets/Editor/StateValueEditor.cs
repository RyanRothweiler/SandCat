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
			stateVal.arrayName = EditorGUILayout.TextField("Array Name", stateVal.entityName);
			stateVal.arrayIndex = EditorGUILayout.IntField("Array Index", stateVal.arrayIndex);
		}

		// For all the options we always want fluent name
		stateVal.fluentName = EditorGUILayout.TextField("Fluent Name", stateVal.fluentName);
	}
}